/**
 *  @file     buffer.cc
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