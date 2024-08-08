#include "BrepLoader.h"

#include <filesystem>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osgUtil/SmoothingVisitor>

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Builder.hxx>
#include <BRep_Tool.hxx>
#include <GeomLib.hxx>
#include <IGESControl_Reader.hxx>
#include <Poly_Connect.hxx>
#include <STEPControl_Reader.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>

#include "Utilities/Text.h"

#include "ModelDefs.h"

namespace glv {
namespace {
bool isStpFile(const std::string& ext) {
    return ext == ".stp" || ext == ".step";
}

bool isIgesFile(const std::string& ext) {
    return ext == ".iges" || ext == ".igs";
}

enum FileType
{
    TYPE_UNKNOW,
    TYPE_STEP,
    TYPE_IGES
};

bool isSupportedType(const std::string& file, FileType& type) {
    namespace fs = std::filesystem;
    fs::path path(file);
    if (!path.has_extension()) return false;

    auto file_ext = path.extension().string();
    std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::tolower);
    if (isStpFile(file_ext))
        type = TYPE_STEP;
    else if (isIgesFile(file_ext))
        type = TYPE_IGES;
    else
        type = TYPE_UNKNOW;

    return type != TYPE_UNKNOW;
}

void getFaceNormals(const TopoDS_Face& theFace, Poly_Triangulation* aPolyTri, TColgp_Array1OfDir& theNormals) {
    Standard_Integer numNodes = aPolyTri->NbNodes();

    if (aPolyTri->HasNormals()) {
        for (Standard_Integer aNodeIter = 1; aNodeIter <= numNodes; ++aNodeIter) {
            theNormals(aNodeIter) = aPolyTri->Normal(aNodeIter);
        }

        if (theFace.Orientation() == TopAbs_REVERSED) {
            for (Standard_Integer aNodeIter = 1; aNodeIter <= numNodes; ++aNodeIter) {
                theNormals.ChangeValue(aNodeIter).Reverse();
            }
        }
    }
    else {
        // take in face the surface location
        Poly_Connect      thePolyConnect(aPolyTri);
        const TopoDS_Face aZeroFace    = TopoDS::Face(theFace.Located(TopLoc_Location()));
        Handle(Geom_Surface) aSurf     = BRep_Tool::Surface(aZeroFace);
        const Standard_Real aTol       = Precision::Confusion();
        Standard_Boolean    hasNodesUV = aPolyTri->HasUVNodes() && !aSurf.IsNull();
        Standard_Integer    aTri[3];

        aPolyTri->AddNormals();
        for (Standard_Integer aNodeIter = 1; aNodeIter <= numNodes; ++aNodeIter) {
            // try to retrieve normal from real surface first, when UV coordinates are available
            if (!hasNodesUV ||
                GeomLib::NormEstim(aSurf, aPolyTri->UVNode(aNodeIter), aTol, theNormals(aNodeIter)) > 1) {
                // compute flat normals
                gp_XYZ eqPlan(0.0, 0.0, 0.0);

                for (thePolyConnect.Initialize(aNodeIter); thePolyConnect.More(); thePolyConnect.Next()) {
                    aPolyTri->Triangle(thePolyConnect.Value()).Get(aTri[0], aTri[1], aTri[2]);
                    const gp_XYZ        v1(aPolyTri->Node(aTri[1]).Coord() - aPolyTri->Node(aTri[0]).Coord());
                    const gp_XYZ        v2(aPolyTri->Node(aTri[2]).Coord() - aPolyTri->Node(aTri[1]).Coord());
                    const gp_XYZ        vv   = v1 ^ v2;
                    const Standard_Real aMod = vv.Modulus();

                    if (aMod >= aTol) {
                        eqPlan += vv / aMod;
                    }
                }

                const Standard_Real aModMax = eqPlan.Modulus();
                theNormals(aNodeIter)       = (aModMax > aTol) ? gp_Dir(eqPlan) : gp::DZ();
            }

            aPolyTri->SetNormal(aNodeIter, theNormals(aNodeIter));
        }

        if (theFace.Orientation() == TopAbs_REVERSED) {
            for (Standard_Integer aNodeIter = 1; aNodeIter <= numNodes; ++aNodeIter) {
                theNormals.ChangeValue(aNodeIter).Reverse();
            }
        }
    }
}
} // namespace

bool BrepLoader::isSupported(const std::string& file) {
    FileType type;
    return isSupportedType(file, type);
}

osg::MatrixTransform* BrepLoader::loadFile(const std::string& file) {
    XSControl_Reader* reader = nullptr;
    FileType          type;
    if (!isSupportedType(file, type)) return nullptr;
    if (type == TYPE_STEP) {
        reader = new STEPControl_Reader();
    }
    else if (type == TYPE_IGES) {
        reader = new IGESControl_Reader();
    }
    if (!reader) return nullptr;
    auto u8path = stdutils::gbkToUtf8(file);
    auto status = reader->ReadFile(u8path.data());
    if (status != IFSelect_RetDone) return nullptr;

    reader->TransferRoots();
    TopoDS_Shape shape;
    if (reader->NbShapes() > 1) {
        BRep_Builder    builder;
        TopoDS_Compound compound;
        builder.MakeCompound(compound);
        for (int i = 1; i <= reader->NbShapes(); i++) {
            builder.Add(compound, reader->Shape(i));
        }
        shape = compound;
    }
    else {
        shape = reader->OneShape();
    }

    delete reader;

    std::set<int>              all_edge_indices;
    TopTools_IndexedMapOfShape face_map;
    TopTools_IndexedMapOfShape edge_map;

    TopExp::MapShapes(shape, TopAbs_FACE, face_map);
    TopExp::MapShapes(shape, TopAbs_EDGE, edge_map);

    for (int i = 1; i <= edge_map.Extent(); i++) {
        all_edge_indices.insert(i);
    }

    auto face_geod   = new osg::Geode();
    auto edge_geod   = new osg::Geode();
    auto face_colors = new osg::Vec4Array();
    auto edge_colors = new osg::Vec4Array();
    face_colors->push_back(osg::Vec4(220.f / 255, 223.f / 255, 227.f / 255, 1.f));
    edge_colors->push_back(osg::Vec4(0.2f, 0.2f, 0.2f, 1.f));

    IMeshTools_Parameters params;
    params.Angle                    = 0.2;
    params.Deflection               = 1;
    params.MinSize                  = 1e-4;
    params.InParallel               = true;
    params.Relative                 = true;
    params.InternalVerticesMode     = true;
    params.ControlSurfaceDeflection = true;
    params.ForceFaceDeflection      = false;
    params.CleanModel               = true;
    params.AllowQualityDecrease     = false;
    params.AdjustMinSize            = true;

    BRepMesh_IncrementalMesh im(shape, params);
    im.Perform();

    for (int i = 1; i <= face_map.Extent(); i++) {
        auto&           face = TopoDS::Face(face_map(i));
        TopLoc_Location loc;
        auto            mesh = BRep_Tool::Triangulation(face, loc);
        if (mesh.IsNull()) continue;

        auto vertices = new osg::Vec3Array();
        auto norms    = new osg::Vec3Array();
        auto indices  = new osg::DrawElementsUInt(GL_TRIANGLES);
        vertices->reserve(mesh->NbNodes());
        norms->reserve(mesh->NbNodes());
        indices->reserve(mesh->NbTriangles() * 3);
        for (auto i = 1; i <= mesh->NbNodes(); i++) {
            auto pnt = mesh->Node(i);
            pnt.Transform(loc.Transformation());
            vertices->push_back(osg::Vec3(pnt.X(), pnt.Y(), pnt.Z()));
        }
        for (auto i = 1; i <= mesh->NbTriangles(); i++) {
            Standard_Integer v1, v2, v3;
            const auto&      tri = mesh->Triangle(i);
            tri.Get(v1, v2, v3);
            if (face.Orientation() == TopAbs_REVERSED) std::swap(v1, v2);
            indices->push_back(v1 - 1);
            indices->push_back(v2 - 1);
            indices->push_back(v3 - 1);
        }
        TColgp_Array1OfDir occ_norms(1, mesh->NbNodes());
        getFaceNormals(face, mesh.get(), occ_norms);
        for (auto& n : occ_norms) {
            n.Transform(loc.Transformation());
            norms->push_back(osg::Vec3(n.X(), n.Y(), n.Z()));
        }

        auto face_geom = new osg::Geometry();
        face_geom->setColorArray(face_colors, osg::Array::BIND_OVERALL);
        face_geom->setVertexArray(vertices);
        face_geom->setNormalArray(norms, osg::Array::BIND_PER_VERTEX);
        face_geom->addPrimitiveSet(indices);
        // osgUtil::SmoothingVisitor().smooth(*face_geom);
        face_geod->addDrawable(face_geom);

        TopExp_Explorer exp(face, TopAbs_EDGE);
        for (; exp.More(); exp.Next()) {
            auto edge       = TopoDS::Edge(exp.Current());
            auto edge_index = edge_map.FindIndex(edge);
            if (all_edge_indices.find(edge_index) == all_edge_indices.end()) continue;
            all_edge_indices.erase(edge_index);
            auto polygon = BRep_Tool::PolygonOnTriangulation(edge, mesh, loc);
            if (polygon.IsNull()) continue;
            auto& nodes     = polygon->Nodes();
            auto  edge_geom = new osg::Geometry();

#if true
            vertices = new osg::Vec3Array();
            for (int i = nodes.Lower(); i <= nodes.Upper(); i++) {
                auto pnt = mesh->Node(nodes(i));
                pnt.Transform(loc.Transformation());
                vertices->push_back(osg::Vec3(pnt.X(), pnt.Y(), pnt.Z()));
            }
            edge_geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, vertices->size()));
#else
            // 有问题
            indices = new osg::DrawElementsUInt(GL_LINE_STRIP);
            for (auto i : nodes) {
                indices->push_back(i);
            }
            edge_geom->addPrimitiveSet(indices);
#endif

            edge_geom->setVertexArray(vertices);
            edge_geom->setColorArray(edge_colors, osg::Array::BIND_OVERALL);

            edge_geom->setUseDisplayList(false);
            edge_geod->addDrawable(edge_geom);
        }
    }

    edge_geod->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(1.0f), 1);
    edge_geod->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);
    face_geod->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonOffset(2.f, 2.f), 1);
    // face_geod->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
    // osg::PolygonMode::LINE));

    auto root = new osg::MatrixTransform();
    root->addChild(face_geod);
    root->addChild(edge_geod);

    setPipelineMask(edge_geod, FORWARD_SCENE_MASK);
    setPipelineMask(face_geod, DEFERRED_SCENE_MASK | SHADOW_CASTER_MASK);

    return root;
}
} // namespace glv
