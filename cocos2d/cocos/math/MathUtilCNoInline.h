//
//  MathUtilCNoInline.h
//  cocos2d_libs
//
//  Created by Kai Guo on 15/11/12.
//
//

#ifndef MathUtilCNoInline_h
#define MathUtilCNoInline_h

#include "CCMathBase.h"

NS_CC_MATH_BEGIN

class MathUtilCNoInline
{
public:
    static void addMatrix(const float* m, float scalar, float* dst);
    
    static void addMatrix(const float* m1, const float* m2, float* dst);
    
    static void subtractMatrix(const float* m1, const float* m2, float* dst);
    
    static void multiplyMatrix(const float* m, float scalar, float* dst);
    
    static void multiplyMatrix(const float* m1, const float* m2, float* dst);
    
    static void negateMatrix(const float* m, float* dst);
    
    static void transposeMatrix(const float* m, float* dst);
    
    static void transformVec4(const float* m, float x, float y, float z, float w, float* dst);
    
    static void transformVec4(const float* m, const float* v, float* dst);
    
    static void crossVec3(const float* v1, const float* v2, float* dst);
};

NS_CC_MATH_END

#endif /* MathUtilCNoInline_h */
