/**
 *  @file     graphics_pipeline_state.cc
 *  @brief    �O���t�B�b�N�X�p�C�v���C���X�e�[�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/21
 *  @version  1.0
 */

 // include
#include <functional>
#include "graphics_pipeline_state.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g�p�N���X
       */
      class GraphicsPipelineState final : public IGraphicsPipelineState
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        GraphicsPipelineState() : pipeline_state_(nullptr, [](ID3D12PipelineState* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~GraphicsPipelineState() override
        {
          Release();
        }

        /**
         *  @brief  �R�}���h���X�g�ɃO���t�B�b�N�X�p�C�v���C���X�e�[�g��ݒ肷��
         *  @param  command_list:�O���t�B�b�N�X�p�C�v���C���X�e�[�g��ݒ肷��R�}���h���X�g
         */
        void SetPipelineState(ID3D12GraphicsCommandList* command_list) const override
        {
          command_list->SetPipelineState(pipeline_state_.get());
        }

        /**
         *  @brief  �p�C�v���C���X�e�[�g��ݒ肷��
         *  @param  pipeline_state:�p�C�v���C���X�e�[�g
         */
        void SetPipelineState(ID3D12PipelineState* pipeline_state)
        {
          pipeline_state_.reset(pipeline_state);
        }

        /**
         *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���擾����
         *  @return �O���t�B�b�N�X�p�C�v���C���X�e�[�g
         */
        const ID3D12PipelineState& Get() const override
        {
          return *pipeline_state_;
        }

        /**
         *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���J������
         */
        void Release() noexcept override
        {
          pipeline_state_.reset();
        }

      private:
        std::unique_ptr<ID3D12PipelineState, std::function<void(ID3D12PipelineState*)>> pipeline_state_;  ///< �p�C�v���C���X�e�[�g
      };
    };

    /**
     *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  graphics_pipeline_state_desc:�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐ݒ�
     *  @return �O���t�B�b�N�X�p�C�v���C���X�e�[�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IGraphicsPipelineState> IGraphicsPipelineState::Create(ID3D12Device* const device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphics_pipeline_state_desc)
    {
      // �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
      ID3D12PipelineState* temporary;
      if (FAILED(device->CreateGraphicsPipelineState(&graphics_pipeline_state_desc, IID_PPV_ARGS(&temporary))))
      {
        ID3D12DebugDevice* debugInterface;
        if (SUCCEEDED(device->QueryInterface(&debugInterface)))
        {
          debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
          debugInterface->Release();
        }

        throw std::exception("�p�C�v���C���X�e�[�g�̐����Ɏ��s���܂����B");
      }

      auto pipeline_state = std::make_shared<GraphicsPipelineState>();
      pipeline_state->SetPipelineState(temporary);
      return pipeline_state;
    }
  };
};