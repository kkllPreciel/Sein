/**
 *  @file     direct3d11_depth_stencil_state.cc
 *  @brief    Direct3D11�̐[�x�X�e���V���X�e�[�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/17
 *  @version  1.0
 */

 // include
#include "direct3d11_depth_stencil_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �[�x�X�e���V���X�e�[�g�p�N���X
       */
      class DepthStencilState final : public IDepthStencilState
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        DepthStencilState() : depth_stencil_state_(nullptr, [](ID3D11DepthStencilState* p) { p->Release(); }), desc_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~DepthStencilState() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  desc:�[�x�X�e���V���̐ݒ�
         */
        void Create(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_DESC& desc)
        {
          ID3D11DepthStencilState* depth_stencil_state;
          if (FAILED(device->CreateDepthStencilState(&desc, &depth_stencil_state)))
          {
            throw std::exception("�[�x�X�e���V���X�e�[�g�̍쐬�Ɏ��s���܂����B");
          }

          depth_stencil_state_.reset(depth_stencil_state);
          desc_ = desc;
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          depth_stencil_state_.reset();
        }

      private:
        std::unique_ptr<ID3D11DepthStencilState, std::function<void(ID3D11DepthStencilState*)>> depth_stencil_state_; ///< �[�x�X�e���V���X�e�[�g
        D3D11_DEPTH_STENCIL_DESC desc_;                                                                               ///< �[�x�X�e���V���̐ݒ�
      };
    };

    /**
     *  @brief  �[�x�X�e���V���X�e�[�g���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  desc:�[�x�X�e���V���̐ݒ�
     *  @return �[�x�X�e���V���ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IDepthStencilState> IDepthStencilState::Create(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_DESC& desc)
    {
      auto depth_stencil_state = std::make_shared<DepthStencilState>();

      depth_stencil_state->Create(device, desc);

      return depth_stencil_state;
    }
  };
};