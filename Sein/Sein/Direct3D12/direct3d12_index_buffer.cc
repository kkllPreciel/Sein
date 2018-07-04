/**
 *  @file     index_buffer.cc
 *  @brief    ���_�C���f�b�N�X�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/03
 *  @version  1.0
 */

 // include
#include "direct3d12_index_buffer.h"
#include "direct3d12_buffer.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  ���_�C���f�b�N�X�o�b�t�@�p�N���X
       */
      class IndexBuffer final : public IIndexBuffer
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        IndexBuffer() : buffer_(nullptr), index_buffer_view_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~IndexBuffer() override
        {
          Release();
        }

        /**
         *  @brief  �o�b�t�@��ݒ肷��
         *  @param  buffer:�o�b�t�@
         */
        void SetBuffer(Buffer* const buffer)
        {
          buffer_.reset(buffer);
        }

        /**
         *  @brief  �}�b�v����
         *  @param  format:���_�C���f�b�N�X�̃t�H�[�}�b�g
         *  @param  indices:���_�C���f�b�N�X�z��ւ̃|�C���^
         */
        void Map(const DXGI_FORMAT format, const void* const indices) override
        {
          // ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�ւ̃|�C���^
          unsigned char* pData;
          if (FAILED(buffer_->Get().Map(
            0,                                  // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
            nullptr,                            // CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
            reinterpret_cast<void**>(&pData)    // ���\�[�X�f�[�^�ւ̃|�C���^
          )))
          {
            throw std::exception("���_�C���f�b�N�X�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B");
          }

          // ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�֒��_�C���f�b�N�X�f�[�^���R�s�[
          std::memcpy(pData, indices, buffer_->Get().GetDesc().Width);

          // ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�ւ̃|�C���^�𖳌��ɂ���
          buffer_->Get().Unmap(
            0,        // �T�u���\�[�X�C���f�b�N�X�ԍ�
            nullptr   // �}�b�v�������郁�����͈̔́ACPU���ύX�����\���̂���̈�(nullptr�͑S�̈�)
          );

          // ���_�C���f�b�N�X�o�b�t�@�̃r���[������������
          index_buffer_view_.BufferLocation = buffer_->Get().GetGPUVirtualAddress();  // �o�b�t�@�̃A�h���X
          index_buffer_view_.SizeInBytes = buffer_->Get().GetDesc().Width;            // �o�b�t�@(�S���_�C���f�b�N�X���v)�̃T�C�Y(�o�C�g�P��)
          index_buffer_view_.Format = format;                                         // �o�b�t�@�̃t�H�[�}�b�g
        }
        
        /**
         *  @brief  ���_�C���f�b�N�X�o�b�t�@�r���[���擾����
         *  @return ���_�C���f�b�N�X�o�b�t�@�r���[
         */
        const D3D12_INDEX_BUFFER_VIEW& GetView() override
        {
          return index_buffer_view_;
        }
        
        /**
         *  @brief  ���_�C���f�b�N�X�o�b�t�@���J������
         */
        void Release() noexcept override
        {
          index_buffer_view_ = {};
          buffer_.reset();
        }

      private:
        std::unique_ptr<Buffer> buffer_;            ///< �o�b�t�@(���\�[�X)
        D3D12_INDEX_BUFFER_VIEW index_buffer_view_; ///< ���_�C���f�b�N�X�o�b�t�@�r���[
      };
    };

    /**
     *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
     *  @return ���_�C���f�b�N�X�o�b�t�@�ւ̃��j�[�N�|�C���^
     */
    std::unique_ptr<IIndexBuffer> IIndexBuffer::Create(ID3D12Device* const device, const std::uint32_t size_in_bytes)
    {
      // �q�[�v�̐ݒ�
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // �q�[�v�̃������v�[��(�s���ɐݒ�)
      properties.CreationNodeMask = 1;                              // ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
      properties.VisibleNodeMask = 1;                               // ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

      auto buffer = new Buffer();
      auto index_buffer = std::make_unique<IndexBuffer>();
      index_buffer->SetBuffer(buffer);

      try
      {
        buffer->Create(device, properties, size_in_bytes, D3D12_RESOURCE_FLAG_NONE);
      }
      catch (const char*)
      {
        throw std::exception("���_�C���f�b�N�X�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B");
      }

      return index_buffer;
    }
  };
};