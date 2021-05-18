
#if !MEGDNN_TEGRA_X1
// ignore warning of cutlass
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#include "src/cuda/conv_bias/implicit_gemm_conv_bias_cutlass_wrapper.cuinl"


// kernel instance "cutlass_simt_s4_ifprop_relu_s8_16x64x8_16x64x8_2_nc4hw4_nhwc" generated by cutlass generator
using Convolution = 
  typename cutlass::conv::device::Convolution<
    int8_t, 
    cutlass::layout::TensorNCxHWx<4>,
    int8_t, 
    cutlass::layout::TensorCxRSKx<4>,
    cutlass::int4b_t, 
    cutlass::layout::TensorNHWC,
    int32_t, 
    cutlass::layout::TensorNHWC, 
    int32_t, 
    cutlass::conv::ConvType::kConvolution, 
    cutlass::arch::OpClassSimt,
    cutlass::arch::Sm75,
    cutlass::gemm::GemmShape<16, 64, 8>,
    cutlass::gemm::GemmShape<16, 64, 8>,
    cutlass::gemm::GemmShape<1, 1, 4>,
    cutlass::epilogue::thread::BiasAddLinearCombinationReluClamp<
      cutlass::int4b_t,
      8,
      int32_t, 
      int32_t, 
      float
    >,
    cutlass::conv::threadblock::ConvolutionFpropNCxHWxThreadblockSwizzle,     
    2,
    4, 
    4, 
    true, 
    cutlass::arch::OpMultiplyAddSaturate>;


template void megdnn::cuda::cutlass_wrapper::cutlass_convolution_wrapper<Convolution>(
        const typename Convolution::ElementSrc* d_src, 
        const typename Convolution::ElementFilter* d_filter, 
        const typename Convolution::ElementBias* d_bias, 
        const typename Convolution::ElementDst* d_z, 
        typename Convolution::ElementDst* d_dst, 
        int* workspace, 
        typename Convolution::ConvolutionParameter const& conv_param, 
        typename Convolution::EpilogueOutputOp::Params const& epilogue, 
        cudaStream_t stream);
#pragma GCC diagnostic pop
#endif
