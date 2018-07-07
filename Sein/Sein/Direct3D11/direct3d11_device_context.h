/**
 *  @file     direct3d11_device_context.h
 *  @brief    Direct3D11�̃f�o�C�X�R���e�L�X�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/06
 *  @version  1.0
 */

#pragma once

// include
#include "../Renderer/device_context.h"
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�̃f�o�C�X�R���e�L�X�g�p�C���^�[�t�F�[�X
     */
    class IDeviceContext : public Renderer::IDeviceContext
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IDeviceContext() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDeviceContext() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDeviceContext(const IDeviceContext& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDeviceContext& operator = (const IDeviceContext& other) = delete;

      /**
       *  @brief  Direct3D11�p�f�o�C�X�R���e�L�X�g���擾����
       *  @return Direct3D11�p�f�o�C�X�R���e�L�X�g
       */
      virtual const ID3D11DeviceContext& GetD3D11DeviceContext() = 0;

      /**
       *  @brief  �f�o�C�X�R���e�L�X�g���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  context_type:�f�o�C�X�R���e�L�X�g�̎��
       *  @return �f�o�C�X�R���e�L�X�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDeviceContext> Create(ID3D11Device* const device, const IDeviceContext::Type& context_type);
    };
  };
};