#include <gtest/gtest.h>
#include <osg/Matrix>

TEST(Matrixd, multi){
    
    osg::Matrix m;
    
    osg::Vec3d v1(0, 10, 0);
    auto m1 = osg::Matrix::rotate(osg::PI_2, osg::Vec3d(0,0,1));
    auto m2 = osg::Matrix::translate(10, 0, 0);

    auto v1_1 = v1 * m1 * m2;
    auto v1_2 = v1 * m2 * m1;

    ASSERT_TRUE(1);
}