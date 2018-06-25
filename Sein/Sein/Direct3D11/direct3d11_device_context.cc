/**
 *  @file     direct3d11_device_context.cc
 *  @brief    Direct3D11�̃f�o�C�X�R���e�L�X�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/07
 *  @version  1.0
 */

 // include
#include "direct3d11_device_context.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      class DeviceContext final : public IDeviceContext
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        DeviceContext() : context_(nullptr, [](ID3D11DeviceContext* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~DeviceContext() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  context_type:�f�o�C�X�R���e�L�X�g�̎��
         *  @param  type:�C���f�b�N�X�̌^
         */
        void Create(ID3D11Device* const device, const IDeviceContext::Type& context_type)
        {
          ID3D11DeviceContext* context;
          if (context_type == Type::kDeferred)
          {
            device->CreateDeferredContext(0, &context);
          }
          else
          {
            device->GetImmediateContext(&context);
          }

          context_.reset(context);
          type_ = context_type;
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          context_->Release();
        }

      private:
        std::unique_ptr<ID3D11DeviceContext, std::function<void(ID3D11DeviceContext*)>> context_; ///< �f�o�C�X�R���e�L�X�g
        Type type_;                                                                               ///< �f�o�C�X�R���e�L�X�g�̎��

      };
    };

    /**
     *  @brief  �f�o�C�X�R���e�L�X�g���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  context_type:�f�o�C�X�R���e�L�X�g�̎��
     *  @return �f�o�C�X�R���e�L�X�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IDeviceContext> IDeviceContext::Create(ID3D11Device* const device, const IDeviceContext::Type& context_type)
    {
      auto device_context = std::make_shared<DeviceContext>();

      device_context->Create(device, context_type);

      return device_context;
    }
  };
};