/**
 *  @file     direct3d11_pipeline_state.cc
 *  @brief    Direct3D11�̃p�C�v���I�C���X�e�[�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/07
 *  @version  1.0
 */

 // include
#include "direct3d11_pipeline_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �p�C�v���C���X�e�[�g�p�N���X
       */
      class PipelineState final : public IPipelineState
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        PipelineState() :
          blend_state_(nullptr, [](ID3D11BlendState* p) { p->Release(); }), rasterizer_state_(nullptr, [](ID3D11RasterizerState* p) { p->Release(); }),
          depth_stencil_state_(nullptr, [](ID3D11DepthStencilState* p) { p->Release(); }), input_layout_(nullptr, [](ID3D11InputLayout* p) { p->Release(); }),
          vertex_shader_(nullptr, [](ID3D11VertexShader* p) { p->Release(); }), pixel_shader_(nullptr, [](ID3D11PixelShader* p) { p->Release(); }),
          geometry_shader_(nullptr, [](ID3D11GeometryShader* p) {p->Release(); }), domain_shader_(nullptr, [](ID3D11DomainShader* p) {p->Release(); }),
          hull_shader_(nullptr, [](ID3D11HullShader* p) {p->Release(); }), compute_shader_(nullptr, [](ID3D11ComputeShader* p) {p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~PipelineState() override
        {
          Destroy();
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          compute_shader_.reset();
          hull_shader_.reset();
          domain_shader_.reset();
          geometry_shader_.reset();
          pixel_shader_.reset();
          vertex_shader_.reset();
          input_layout_.reset();
          depth_stencil_state_.reset();
          rasterizer_state_.reset();
          blend_state_.reset();
        }

      private:
        std::unique_ptr<ID3D11BlendState, std::function<void(ID3D11BlendState*)>> blend_state_;                       ///< �u�����h�X�e�[�g
        std::unique_ptr<ID3D11RasterizerState, std::function<void(ID3D11RasterizerState*)>> rasterizer_state_;        ///< ���X�^���C�U�[�X�e�[�g
        std::unique_ptr<ID3D11DepthStencilState, std::function<void(ID3D11DepthStencilState*)>> depth_stencil_state_; ///< �[�x�X�e���V���X�e�[�g
        std::unique_ptr<ID3D11InputLayout, std::function<void(ID3D11InputLayout*)>> input_layout_;                    ///< �C���v�b�g���C�A�E�g
        std::unique_ptr<ID3D11VertexShader, std::function<void(ID3D11VertexShader*)>> vertex_shader_;                 ///< ���_�V�F�[�_�[
        std::unique_ptr<ID3D11PixelShader, std::function<void(ID3D11PixelShader*)>> pixel_shader_;                    ///< �s�N�Z���V�F�[�_�[
        std::unique_ptr<ID3D11GeometryShader, std::function<void(ID3D11GeometryShader*)>> geometry_shader_;           ///< �W�I���g���V�F�[�_�[
        std::unique_ptr<ID3D11DomainShader, std::function<void(ID3D11DomainShader*)>> domain_shader_;                 ///< �h���C���V�F�[�_�[
        std::unique_ptr<ID3D11HullShader, std::function<void(ID3D11HullShader*)>> hull_shader_;                       ///< �n���V�F�[�_�[
        std::unique_ptr<ID3D11ComputeShader, std::function<void(ID3D11ComputeShader*)>> compute_shader_;              ///< �R���s���[�g�V�F�[�_�[
      };
    };

    /**
     *  @brief  �p�C�v���C���X�e�[�g���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @return �p�C�v���C���X�e�[�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IPipelineState> IPipelineState::Create(ID3D11Device* const device)
    {
      auto pipeline_state = std::make_shared<PipelineState>();

      // TODO:��������

      return pipeline_state;
    }
  };
};