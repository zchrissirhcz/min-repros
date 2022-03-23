#include <vector>

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON
#include <limits.h>
#include <math.h>

void warpaffine_bilinear_c4(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride, const float* tm, int type, unsigned int v)
{
    const unsigned char* border_color = (const unsigned char*)&v;
    const int wgap = stride - w * 4;

    const unsigned char* src0 = src;
    unsigned char* dst0 = dst;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X), SHRT_MIN), SHRT_MAX)
#define SATURATE_CAST_INT(X)   (int)::std::min(::std::max((int)((X) + ((X) >= 0.f ? 0.5f : -0.5f)), INT_MIN), INT_MAX)

    std::vector<int> adelta(w);
    std::vector<int> bdelta(w);
    for (int x = 0; x < w; x++)
    {
        adelta[x] = SATURATE_CAST_INT(tm[0] * x * (1 << 10));
        bdelta[x] = SATURATE_CAST_INT(tm[3] * x * (1 << 10));
    }

    int y = 0;
    for (; y < h; y++)
    {
        int X0 = SATURATE_CAST_INT((tm[1] * y + tm[2]) * (1 << 10));
        int Y0 = SATURATE_CAST_INT((tm[4] * y + tm[5]) * (1 << 10));

        int x = 0;
        for (; x + 7 < w; x += 8)
        {
            int sxy_inout = 0;
            {
                int X_0 = X0 + adelta[x];
                int Y_0 = Y0 + bdelta[x];
                int X_7 = X0 + adelta[x + 7];
                int Y_7 = Y0 + bdelta[x + 7];

                short sx_0 = SATURATE_CAST_SHORT((X_0 >> 10));
                short sy_0 = SATURATE_CAST_SHORT((Y_0 >> 10));
                short sx_7 = SATURATE_CAST_SHORT((X_7 >> 10));
                short sy_7 = SATURATE_CAST_SHORT((Y_7 >> 10));

                if (((unsigned short)sx_0 < srcw - 1 && (unsigned short)sy_0 < srch - 1) && ((unsigned short)sx_7 < srcw - 1 && (unsigned short)sy_7 < srch - 1))
                {
                    // all inside
                    sxy_inout = 1;
                }
                else if ((sx_0 < -1 && sx_7 < -1) || (sx_0 >= srcw && sx_7 >= srcw) || (sy_0 < -1 && sy_7 < -1) || (sy_0 >= srch && sy_7 >= srch))
                {
                    // all outside
                    sxy_inout = 2;
                }
            }

            if (sxy_inout == 1)
            {
                // all inside
#if __ARM_NEON
                int32x4_t _Xl = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x));
                int32x4_t _Xh = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x + 4));
                int32x4_t _Yl = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x));
                int32x4_t _Yh = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x + 4));

                int16x4_t _sxl = vqshrn_n_s32(_Xl, 10);
                int16x4_t _sxh = vqshrn_n_s32(_Xh, 10);
                int16x4_t _syl = vqshrn_n_s32(_Yl, 10);
                int16x4_t _syh = vqshrn_n_s32(_Yh, 10);

                uint32x4_t _v1024m1 = vdupq_n_u32((1 << 10) - 1);
                uint16x8_t _fx = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xh), _v1024m1)));
                uint16x8_t _fy = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yh), _v1024m1)));

                uint16x8_t _alpha0 = vsubq_u16(vdupq_n_u16(1 << 10), _fx);
                uint16x8_t _alpha1 = _fx;
                uint16x8_t _beta0 = vsubq_u16(vdupq_n_u16(1 << 10), _fy);
                uint16x8_t _beta1 = _fy;

                int16x4_t _srcstride = vdup_n_s16(srcstride);
                int16x4_t _v4 = vdup_n_s16(4);

                int32x4_t _a0l = vmlal_s16(vmull_s16(_srcstride, _syl), _sxl, _v4);
                int32x4_t _a0h = vmlal_s16(vmull_s16(_srcstride, _syh), _sxh, _v4);
                int32x4_t _b0l = vaddw_s16(_a0l, _srcstride);
                int32x4_t _b0h = vaddw_s16(_a0h, _srcstride);

                uint8x8x4_t _a0 = uint8x8x4_t();
                uint8x8x4_t _a1 = uint8x8x4_t();
                uint8x8x4_t _b0 = uint8x8x4_t();
                uint8x8x4_t _b1 = uint8x8x4_t();
                {
                    uint8x8_t _a0a1_0 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 0));
                    uint8x8_t _a0a1_1 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 1));
                    uint8x8_t _a0a1_2 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 2));
                    uint8x8_t _a0a1_3 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 3));
                    uint8x8_t _a0a1_4 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 0));
                    uint8x8_t _a0a1_5 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 1));
                    uint8x8_t _a0a1_6 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 2));
                    uint8x8_t _a0a1_7 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 3));

                    // transpose 8x8
                    uint8x8x2_t _a0a101t_r = vtrn_u8(_a0a1_0, _a0a1_1);
                    uint8x8x2_t _a0a123t_r = vtrn_u8(_a0a1_2, _a0a1_3);
                    uint8x8x2_t _a0a145t_r = vtrn_u8(_a0a1_4, _a0a1_5);
                    uint8x8x2_t _a0a167t_r = vtrn_u8(_a0a1_6, _a0a1_7);

                    uint16x4x2_t _a0a102tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a101t_r.val[0]), vreinterpret_u16_u8(_a0a123t_r.val[0]));
                    uint16x4x2_t _a0a113tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a101t_r.val[1]), vreinterpret_u16_u8(_a0a123t_r.val[1]));
                    uint16x4x2_t _a0a146tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a145t_r.val[0]), vreinterpret_u16_u8(_a0a167t_r.val[0]));
                    uint16x4x2_t _a0a157tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a145t_r.val[1]), vreinterpret_u16_u8(_a0a167t_r.val[1]));

                    uint32x2x2_t _a0a104ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a102tt_r.val[0]), vreinterpret_u32_u16(_a0a146tt_r.val[0]));
                    uint32x2x2_t _a0a115ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a113tt_r.val[0]), vreinterpret_u32_u16(_a0a157tt_r.val[0]));
                    uint32x2x2_t _a0a126ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a102tt_r.val[1]), vreinterpret_u32_u16(_a0a146tt_r.val[1]));
                    uint32x2x2_t _a0a137ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a113tt_r.val[1]), vreinterpret_u32_u16(_a0a157tt_r.val[1]));

                    _a0.val[0] = vreinterpret_u8_u32(_a0a104ttt_r.val[0]);
                    _a0.val[1] = vreinterpret_u8_u32(_a0a115ttt_r.val[0]);
                    _a0.val[2] = vreinterpret_u8_u32(_a0a126ttt_r.val[0]);
                    _a0.val[3] = vreinterpret_u8_u32(_a0a137ttt_r.val[0]);
                    _a1.val[0] = vreinterpret_u8_u32(_a0a104ttt_r.val[1]);
                    _a1.val[1] = vreinterpret_u8_u32(_a0a115ttt_r.val[1]);
                    _a1.val[2] = vreinterpret_u8_u32(_a0a126ttt_r.val[1]);
                    _a1.val[3] = vreinterpret_u8_u32(_a0a137ttt_r.val[1]);

                    uint8x8_t _b0b1_0 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 0));
                    uint8x8_t _b0b1_1 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 1));
                    uint8x8_t _b0b1_2 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 2));
                    uint8x8_t _b0b1_3 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 3));
                    uint8x8_t _b0b1_4 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 0));
                    uint8x8_t _b0b1_5 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 1));
                    uint8x8_t _b0b1_6 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 2));
                    uint8x8_t _b0b1_7 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 3));

                    // transpose 8x8
                    uint8x8x2_t _b0b101t_r = vtrn_u8(_b0b1_0, _b0b1_1);
                    uint8x8x2_t _b0b123t_r = vtrn_u8(_b0b1_2, _b0b1_3);
                    uint8x8x2_t _b0b145t_r = vtrn_u8(_b0b1_4, _b0b1_5);
                    uint8x8x2_t _b0b167t_r = vtrn_u8(_b0b1_6, _b0b1_7);

                    uint16x4x2_t _b0b102tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b101t_r.val[0]), vreinterpret_u16_u8(_b0b123t_r.val[0]));
                    uint16x4x2_t _b0b113tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b101t_r.val[1]), vreinterpret_u16_u8(_b0b123t_r.val[1]));
                    uint16x4x2_t _b0b146tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b145t_r.val[0]), vreinterpret_u16_u8(_b0b167t_r.val[0]));
                    uint16x4x2_t _b0b157tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b145t_r.val[1]), vreinterpret_u16_u8(_b0b167t_r.val[1]));

                    uint32x2x2_t _b0b104ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b102tt_r.val[0]), vreinterpret_u32_u16(_b0b146tt_r.val[0]));
                    uint32x2x2_t _b0b115ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b113tt_r.val[0]), vreinterpret_u32_u16(_b0b157tt_r.val[0]));
                    uint32x2x2_t _b0b126ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b102tt_r.val[1]), vreinterpret_u32_u16(_b0b146tt_r.val[1]));
                    uint32x2x2_t _b0b137ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b113tt_r.val[1]), vreinterpret_u32_u16(_b0b157tt_r.val[1]));

                    _b0.val[0] = vreinterpret_u8_u32(_b0b104ttt_r.val[0]);
                    _b0.val[1] = vreinterpret_u8_u32(_b0b115ttt_r.val[0]);
                    _b0.val[2] = vreinterpret_u8_u32(_b0b126ttt_r.val[0]);
                    _b0.val[3] = vreinterpret_u8_u32(_b0b137ttt_r.val[0]);
                    _b1.val[0] = vreinterpret_u8_u32(_b0b104ttt_r.val[1]);
                    _b1.val[1] = vreinterpret_u8_u32(_b0b115ttt_r.val[1]);
                    _b1.val[2] = vreinterpret_u8_u32(_b0b126ttt_r.val[1]);
                    _b1.val[3] = vreinterpret_u8_u32(_b0b137ttt_r.val[1]);
                }

                uint16x8_t _a0_0 = vmovl_u8(_a0.val[0]);
                uint16x8_t _a0_1 = vmovl_u8(_a0.val[1]);
                uint16x8_t _a0_2 = vmovl_u8(_a0.val[2]);
                uint16x8_t _a0_3 = vmovl_u8(_a0.val[3]);
                uint16x8_t _a1_0 = vmovl_u8(_a1.val[0]);
                uint16x8_t _a1_1 = vmovl_u8(_a1.val[1]);
                uint16x8_t _a1_2 = vmovl_u8(_a1.val[2]);
                uint16x8_t _a1_3 = vmovl_u8(_a1.val[3]);
                uint16x8_t _b0_0 = vmovl_u8(_b0.val[0]);
                uint16x8_t _b0_1 = vmovl_u8(_b0.val[1]);
                uint16x8_t _b0_2 = vmovl_u8(_b0.val[2]);
                uint16x8_t _b0_3 = vmovl_u8(_b0.val[3]);
                uint16x8_t _b1_0 = vmovl_u8(_b1.val[0]);
                uint16x8_t _b1_1 = vmovl_u8(_b1.val[1]);
                uint16x8_t _b1_2 = vmovl_u8(_b1.val[2]);
                uint16x8_t _b1_3 = vmovl_u8(_b1.val[3]);

                uint16x4_t _a00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_0), vget_low_u16(_alpha0)), vget_low_u16(_a1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_1), vget_low_u16(_alpha0)), vget_low_u16(_a1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_2), vget_low_u16(_alpha0)), vget_low_u16(_a1_2), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_3l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_3), vget_low_u16(_alpha0)), vget_low_u16(_a1_3), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_0), vget_high_u16(_alpha0)), vget_high_u16(_a1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_1), vget_high_u16(_alpha0)), vget_high_u16(_a1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_2), vget_high_u16(_alpha0)), vget_high_u16(_a1_2), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_3h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_3), vget_high_u16(_alpha0)), vget_high_u16(_a1_3), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_0), vget_low_u16(_alpha0)), vget_low_u16(_b1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_1), vget_low_u16(_alpha0)), vget_low_u16(_b1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_2), vget_low_u16(_alpha0)), vget_low_u16(_b1_2), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_3l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_3), vget_low_u16(_alpha0)), vget_low_u16(_b1_3), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_0), vget_high_u16(_alpha0)), vget_high_u16(_b1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_1), vget_high_u16(_alpha0)), vget_high_u16(_b1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_2), vget_high_u16(_alpha0)), vget_high_u16(_b1_2), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_3h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_3), vget_high_u16(_alpha0)), vget_high_u16(_b1_3), vget_high_u16(_alpha1)), 5);

                uint16x4_t _dst_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0l, vget_low_u16(_beta0)), _b00_0l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1l, vget_low_u16(_beta0)), _b00_1l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_2l, vget_low_u16(_beta0)), _b00_2l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_3l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_3l, vget_low_u16(_beta0)), _b00_3l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0h, vget_high_u16(_beta0)), _b00_0h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1h, vget_high_u16(_beta0)), _b00_1h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_2h, vget_high_u16(_beta0)), _b00_2h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_3h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_3h, vget_high_u16(_beta0)), _b00_3h, vget_high_u16(_beta1)), 15);

                uint8x8x4_t _dst;
                _dst.val[0] = vqmovn_u16(vcombine_u16(_dst_0l, _dst_0h));
                _dst.val[1] = vqmovn_u16(vcombine_u16(_dst_1l, _dst_1h));
                _dst.val[2] = vqmovn_u16(vcombine_u16(_dst_2l, _dst_2h));
                _dst.val[3] = vqmovn_u16(vcombine_u16(_dst_3l, _dst_3h));

                vst4_u8(dst0, _dst);

                dst0 += 4 * 8;
#else
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    short fx = X & ((1 << 10) - 1);
                    short fy = Y & ((1 << 10) - 1);

                    short alpha0 = (1 << 10) - fx;
                    short alpha1 = fx;

                    short beta0 = (1 << 10) - fy;
                    short beta1 = fy;

                    const unsigned char* a0 = src0 + srcstride * sy + sx * 4;
                    const unsigned char* a1 = src0 + srcstride * sy + sx * 4 + 4;
                    const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 4;
                    const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 4 + 4;

                    dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[3] = (unsigned char)(((((unsigned short)((a0[3] * alpha0 + a1[3] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[3] * alpha0 + b1[3] * alpha1) >> 5) * beta1))) >> 15);

                    dst0 += 4;
                }
#endif // __ARM_NEON
            }
            else if (sxy_inout == 2)
            {
                // all outside
                if (type != -233)
                {
#if __ARM_NEON
                    uint8x8x4_t _border_color;
                    _border_color.val[0] = vdup_n_u8(border_color[0]);
                    _border_color.val[1] = vdup_n_u8(border_color[1]);
                    _border_color.val[2] = vdup_n_u8(border_color[2]);
                    _border_color.val[3] = vdup_n_u8(border_color[3]);

                    vst4_u8(dst0, _border_color);
#else
                    for (int xi = 0; xi < 8; xi++)
                    {
                        dst0[xi * 4] = border_color[0];
                        dst0[xi * 4 + 1] = border_color[1];
                        dst0[xi * 4 + 2] = border_color[2];
                        dst0[xi * 4 + 3] = border_color[3];
                    }
#endif // __ARM_NEON
                }
                else
                {
                    // skip
                }

                dst0 += 32;
            }
            else // if (sxy_inout == 0)
            {
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
                    {
                        dst0[0] = border_color[0];
                        dst0[1] = border_color[1];
                        dst0[2] = border_color[2];
                        dst0[3] = border_color[3];
                    }
                    else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
                    {
                        // skip
                    }
                    else
                    {
                        short fx = X & ((1 << 10) - 1);
                        short fy = Y & ((1 << 10) - 1);

                        short alpha0 = (1 << 10) - fx;
                        short alpha1 = fx;

                        short beta0 = (1 << 10) - fy;
                        short beta1 = fy;

                        short sx1 = sx + 1;
                        short sy1 = sy + 1;

                        const unsigned char* a0 = src0 + srcstride * sy + sx * 4;
                        const unsigned char* a1 = src0 + srcstride * sy + sx * 4 + 4;
                        const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 4;
                        const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 4 + 4;

                        if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                        {
                            a0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                        {
                            a1 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b1 = type != -233 ? border_color : dst0;
                        }

                        dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[3] = (unsigned char)(((((unsigned short)((a0[3] * alpha0 + a1[3] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[3] * alpha0 + b1[3] * alpha1) >> 5) * beta1))) >> 15);
                    }

                    dst0 += 4;
                }
            }
        }
        for (; x < w; x++)
        {
            int X = X0 + adelta[x];
            int Y = Y0 + bdelta[x];

            short sx = SATURATE_CAST_SHORT((X >> 10));
            short sy = SATURATE_CAST_SHORT((Y >> 10));

            if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
            {
                dst0[0] = border_color[0];
                dst0[1] = border_color[1];
                dst0[2] = border_color[2];
                dst0[3] = border_color[3];
            }
            else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
            {
                // skip
            }
            else
            {
                short fx = X & ((1 << 10) - 1);
                short fy = Y & ((1 << 10) - 1);

                short alpha0 = (1 << 10) - fx;
                short alpha1 = fx;

                short beta0 = (1 << 10) - fy;
                short beta1 = fy;

                short sx1 = sx + 1;
                short sy1 = sy + 1;

                const unsigned char* a0 = src0 + srcstride * sy + sx * 4;
                const unsigned char* a1 = src0 + srcstride * sy + sx * 4 + 4;
                const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 4;
                const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 4 + 4;

                if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                {
                    a0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                {
                    a1 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                {
                    b0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                {
                    b1 = type != -233 ? border_color : dst0;
                }

                dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                dst0[3] = (unsigned char)(((((unsigned short)((a0[3] * alpha0 + a1[3] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[3] * alpha0 + b1[3] * alpha1) >> 5) * beta1))) >> 15);
            }

            dst0 += 4;
        }

        dst0 += wgap;
    }

#undef SATURATE_CAST_SHORT
#undef SATURATE_CAST_INT
}

