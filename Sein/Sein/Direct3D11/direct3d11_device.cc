/**
 *  @file     direct3d11_device.cc
 *  @brief    Direct3D11�̃f�o�C�X�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

 // include
#include "direct3d11_device.h"
#include <cassert>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �f�o�C�X�p�N���X
       */
      class Device final : public IDevice
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Device()
          : device_(nullptr, [](ID3D11Device* p) { p->Release(); }),
          context_(nullptr, [](ID3D11DeviceContext* p) { p->Release(); }),
          swap_chain_(nullptr, [](IDXGISwapChain* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Device() override
        {
          Destroy();
        }

        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          swap_chain_.reset();
          context_.reset();
          device_.reset();
        }

        /**
         *  @brief  �쐬����
         *  @param  handle:�E�B���h�E�n���h��
         *  @param  width:�E�B���h�E�̉���
         *  @param  height:�E�B���h�E�̏c��
         */
        void Create(const HWND handle, const std::uint32_t width, const std::uint32_t height)
        {
          assert(device_ == nullptr);
        }

      private:
        std::shared_ptr<ID3D11Device> device_;          ///< �f�o�C�X
        std::shared_ptr<ID3D11DeviceContext> context_;  ///< �R���e�L�X�g
        std::shared_ptr<IDXGISwapChain> swap_chain_;    ///< �X���b�v�`�F�[��
        D3D_DRIVER_TYPE driver_type_;                   ///< �h���C�o�[���
        D3D_FEATURE_LEVEL feature_level_;               ///< �@�\���x��
      };
    };

    /**
     *  @brief  �f�o�C�X���쐬����
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  width:�E�B���h�E�̉���
     *  @param  height:�E�B���h�E�̏c��
     */
    std::shared_ptr<IDevice> IDevice::Create(HWND handle, std::uint32_t width, std::uint32_t height)
    {
      auto device = std::make_shared<Device>();
    
      device->Create(handle, width, height);
    
      return device;
    }
  };
};