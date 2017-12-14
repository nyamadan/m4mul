#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

#if defined(EMSCRIPTEN)
#define aligned_alloc(align, size) (void*)(((uintptr_t)malloc((size) + ((align)-1)) + ((align)-1)) & (~((align)-1)))
#endif

extern "C" {
    void PrintMatrix(float *p);
    float *CreateMatrix(
        float m00, float m10, float m20, float m30,
        float m01, float m11, float m21, float m31,
        float m02, float m12, float m22, float m32,
        float m03, float m13, float m23, float m33
    );
    void TransposeMatrix(float *p);
    void MultiplyMatrix(float *a, float *b, float *dst);
    void ReleaseMatrix(float *p);

    void PrintMatrix(float *p)
    {
        fprintf(stdout, "%8.3f,\t%8.3f,\t%8.3f,\t%8.3f\n%8.3f,\t%8.3f,\t%8.3f,\t%8.3f\n%8.3f,\t%8.3f,\t%8.3f,\t%8.3f\n%8.3f,\t%8.3f,\t%8.3f,\t%8.3f\n",
            p[0], p[4], p[8], p[12],
            p[1], p[5], p[9], p[13],
            p[2], p[6], p[10], p[14],
            p[3], p[7], p[11], p[15]);
    }

    float *CreateMatrix(
        float m00, float m10, float m20, float m30,
        float m01, float m11, float m21, float m31,
        float m02, float m12, float m22, float m32,
        float m03, float m13, float m23, float m33
    )
    {
        auto p = static_cast<float*>(aligned_alloc(16, sizeof(float) * 16));

        p[0] = m00;
        p[1] = m10;
        p[2] = m20;
        p[3] = m30;

        p[4] = m01;
        p[5] = m11;
        p[6] = m21;
        p[7] = m31;

        p[8] = m02;
        p[9] = m12;
        p[10] = m22;
        p[11] = m32;

        p[12] = m03;
        p[13] = m13;
        p[14] = m23;
        p[15] = m33;

        return p;
    }

    void TransposeMatrix(float *p)
    {
#if defined(__SSE__)
        auto m = (__m128 *)p;
        _MM_TRANSPOSE4_PS(m[0], m[1], m[2], m[3]);
#else
        fprintf(stderr, "Not implemented.\n");
#endif
    }

    void MultiplyMatrix(float *a, float *b, float *dst)
    {
#if defined(__SSE__)
        __m128 *_a = (__m128 *)a;
        __m128 *_b = (__m128 *)b;

        // Transpose
        __m128 tmp[4];
        for(auto i = 0; i < 4; i++)
        {
            tmp[i] = _mm_load_ps((float *)&_a[i]);
        }
        TransposeMatrix((float *)&tmp[0]);
        _a = tmp;

        __m128 o;
        float *f = (float *)&o;

        for(auto x = 0; x < 4; x++) {
            for(auto y = 0; y < 4; y++) {
                o = _mm_mul_ps(_a[y], _b[x]);
                dst[x * 4 + y] = f[0] + f[1] + f[2] + f[3];
            }
        }
#else
        for(auto x = 0; x < 4; x++) {
            for(auto y = 0; y < 4; y++) {
                auto i = x * 4;
                dst[i + y] =
                      a[0 + y] * b[i + 0] 
                    + a[4 + y] * b[i + 1] 
                    + a[8 + y] * b[i + 2] 
                    + a[12 + y] * b[i + 3];
            }
        }
#endif
    }

    void ReleaseMatrix(float *p)
    {
        std::free(p);
    }
}
