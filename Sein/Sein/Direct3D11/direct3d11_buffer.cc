/**
 *  @file     direct3d11_buffer.cc
 *  @brief    Direct3D11�̃o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

 // include
#include "direct3d11_buffer.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �o�b�t�@�p�N���X
       */
      class Buffer final : public IBuffer
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Buffer() : buffer_(nullptr, [](ID3D11Buffer* p) { p->Release(); }), buffer_desc_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Buffer() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  buffer_desc:�o�b�t�@�̐ݒ�
         */
        void Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc)
        {
          ID3D11Buffer* buffer;
          if (FAILED(device->CreateBuffer(&buffer_desc, nullptr, &buffer)))
          {
            throw std::exception("�o�b�t�@�̍쐬�Ɏ��s���܂����B");
          }

          buffer_.reset(buffer);
          buffer_desc_ = buffer_desc;
        }

        /**
         *  @brief  �}�b�v����
         *  @param  context:�R���e�L�X�g
         *  @param  subresource:�T�u���\�[�X�̃C���f�b�N�X�ԍ�
         *  @param  map_type:���\�[�X�ɑ΂���CPU�̃A�N�Z�X���ݒ�
         *  @param  map_flags:GPU�Ŏg�p���������ꍇ��CPU�̑Ή����@
         *  @param  mapped_resource:�}�b�v���ꂽ�T�u���\�[�X�ւ̃|�C���^
         */
        void Map(ID3D11DeviceContext* const context, UINT subresource, D3D11_MAP map_type, UINT map_flags, D3D11_MAPPED_SUBRESOURCE* const mapped_resource) override
        {
          context->Map(buffer_.get(), subresource, map_type, map_flags, mapped_resource);
        }

        /**
         *  @brief  �A���}�b�v����
         *  @param  context:�R���e�L�X�g
         *  @param  subresource:�A���}�b�v����T�u���\�[�X�̃C���f�b�N�X�ԍ�
         */
        void Unmap(ID3D11DeviceContext* const context, UINT subresource) override
        {
          context->Unmap(buffer_.get(), subresource);
        }

        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          buffer_desc_ = {};
          buffer_.reset();
        }

      private:
        std::unique_ptr<ID3D11Buffer, std::function<void(ID3D11Buffer*)>> buffer_;  ///< �o�b�t�@
        D3D11_BUFFER_DESC buffer_desc_;                                             ///< �o�b�t�@�̐ݒ�
      };
    };

    /**
     *  @brief  �o�b�t�@���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  buffer_desc:�o�b�t�@�̐ݒ�
     *  @return �o�b�t�@�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IBuffer> IBuffer::Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc)
    {
      auto buffer = std::make_shared<Buffer>();

      buffer->Create(device, buffer_desc);

      return buffer;
    }
  };
};