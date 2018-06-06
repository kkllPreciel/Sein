/**
 *  @file     direct3d11_index_buffer.cc
 *  @brief    Direct3D11�̃C���f�b�N�X�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/06
 *  @version  1.0
 */

 // include
#include "direct3d11_index_buffer.h"
#include <functional>
#include "direct3d11_buffer.h"

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �C���f�b�N�X�o�b�t�@�p�N���X
       */
      class IndexBuffer final : public IIndexBuffer
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        IndexBuffer() : buffer_(nullptr)
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~IndexBuffer() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  size_in_bytes:�C���f�b�N�X�z��̃T�C�Y
         *  @param  type:�C���f�b�N�X�̌^
         */
        void Create(ID3D11Device* const device, const std::uint32_t size_in_bytes, Type type)
        {
          D3D11_BUFFER_DESC buffer_desc = {};
          buffer_desc.ByteWidth = size_in_bytes;                ///< �o�b�t�@�̃T�C�Y
          buffer_desc.Usage = D3D11_USAGE_DYNAMIC;              ///< ���I�ɍX�V�\
          buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;      ///< �C���f�b�N�X�o�b�t�@
          buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  ///< CPU����A�N�Z�X�\
          buffer_desc.MiscFlags = 0;                            ///< ���̑��̃t���O

          // �o�b�t�@�̍쐬
          buffer_ = IBuffer::Create(device, buffer_desc);

          type_ = type;
        }

        /**
         *  @brief  �}�b�v����
         *  @param  context:�R���e�L�X�g
         *  @param  size_in_bytes:�C���f�b�N�X�z��̃T�C�Y
         *  @param  indices:�C���f�b�N�X�z��ւ̃|�C���^
         */
        void Map(ID3D11DeviceContext* const context, const std::uint32_t size_in_bytes, const void* const indices) override
        {
          D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
          mapped_buffer.pData = nullptr;

          buffer_->Map(context, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

          std::memcpy(mapped_buffer.pData, indices, size_in_bytes);

          buffer_->Unmap(context, 0);
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          buffer_.reset();
        }

      private:
        std::shared_ptr<IBuffer> buffer_; ///< �o�b�t�@
        Type type_;                       ///< �C���f�b�N�X�̌^
      };
    };

    /**
     *  @brief  �C���f�b�N�X�o�b�t�@���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  size_in_bytes:�C���f�b�N�X�o�b�t�@�̃T�C�Y(�C���f�b�N�X�T�C�Y * �C���f�b�N�X��)
     *  @param  index_type:�C���f�b�N�X�̌^
     *  @return �C���f�b�N�X�o�b�t�@�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(ID3D11Device* const device, const std::uint32_t size_in_bytes, IIndexBuffer::Type index_type)
    {
      auto index_buffer = std::make_shared<IndexBuffer>();

      index_buffer->Create(device, size_in_bytes, index_type);

      return index_buffer;
    }
  };
};