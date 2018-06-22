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

#include "direct3d11_input_layout.h"

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
        PipelineState() : desc_()
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
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  desc:�p�C�v���C���̐ݒ�
         */
        void Create(ID3D11Device* const device, const Desc& desc)
        {
          blend_state_ = IBlendState::Create(device, desc.blend_desc);
          rasterizer_state_ = IRasterizerState::Create(device, desc.rasterizer_desc);
          depth_stencil_state_ = IDepthStencilState::Create(device, desc.depth_stencil_desc);

          if (desc.vertex_shader)
          {
            input_layout_ = IInputLayout::Create(device, desc.input_element_desc_list, desc.vertex_shader->GetDesc());
          }
          else if (desc.compute_shader)
          {
            input_layout_ = IInputLayout::Create(device, desc.input_element_desc_list, desc.compute_shader->GetDesc());
          }
          else
          {
            throw std::exception("���_�V�F�[�_�[�A�R���s���[�g�V�F�[�_�[�̂ǂ�����ݒ肳��Ă��܂���B");
          }

          vertex_shader_ = desc.vertex_shader;
          pixel_shader_ = desc.pixel_shader;
          geometry_shader_ = desc.geometry_shader;
          domain_shader_ = desc.domain_shader;
          hull_shader_ = desc.hull_shader;
          compute_shader_ = desc.compute_shader;
          desc_ = desc;
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
        std::shared_ptr<IBlendState> blend_state_ = nullptr;                ///< �u�����h�X�e�[�g
        std::shared_ptr<IRasterizerState> rasterizer_state_ = nullptr;      ///< ���X�^���C�U�X�e�[�g
        std::shared_ptr<IDepthStencilState> depth_stencil_state_ = nullptr; ///< �[�x�X�e���V���X�e�[�g
        std::shared_ptr<IInputLayout> input_layout_ = nullptr;              ///< ���̓��C�A�E�g
        std::shared_ptr<IShader> vertex_shader_ = nullptr;                  ///< ���_�V�F�[�_�[
        std::shared_ptr<IShader> pixel_shader_ = nullptr;                   ///< �s�N�Z���V�F�[�_�[
        std::shared_ptr<IShader> geometry_shader_ = nullptr;                ///< �W�I���g���V�F�[�_�[
        std::shared_ptr<IShader> domain_shader_ = nullptr;                  ///< �h���C���V�F�[�_�[
        std::shared_ptr<IShader> hull_shader_ = nullptr;                    ///< �n���V�F�[�_�[
        std::shared_ptr<IShader> compute_shader_ = nullptr;                 ///< �R���s���[�g�V�F�[�_�[
        Desc desc_;                                                         ///< �p�C�v���C���̐ݒ�
      };
    };

    /**
     *  @brief  �p�C�v���C���X�e�[�g���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  desc:�p�C�v���C���̐ݒ�
     *  @return �p�C�v���C���X�e�[�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IPipelineState> IPipelineState::Create(ID3D11Device* const device, const Desc& desc)
    {
      auto pipeline_state = std::make_shared<PipelineState>();

      pipeline_state->Create(device, desc);

      return pipeline_state;
    }
  };
};