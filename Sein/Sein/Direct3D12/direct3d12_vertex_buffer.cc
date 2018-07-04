/**
 *  @file     vertex_buffer.cc
 *  @brief    ���_�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/02
 *  @version  1.0
 */

 // include
#include "direct3d12_vertex_buffer.h"
#include "direct3d12_buffer.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  ���_�o�b�t�@�p�N���X
       */
      class VertexBuffer final : public IVertexBuffer
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        VertexBuffer() : buffer_(nullptr), vertex_buffer_view_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~VertexBuffer() override
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
         *  @param  stride_in_bytes:1���_�̃T�C�Y
         *  @param  vertices:���_�z��ւ̃|�C���^
         */
        void Map(const std::uint32_t stride_in_bytes, const void* const vertices) override
        {
          // ���_�o�b�t�@(���\�[�X)�ւ̃|�C���^
          unsigned char* pData;
          if (FAILED(buffer_->Get().Map(
            0,                                  // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
            nullptr,                            // CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
            reinterpret_cast<void**>(&pData)    // ���\�[�X�f�[�^�ւ̃|�C���^
          )))
          {
            throw std::exception("���_�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B");
          }

          // ���_�o�b�t�@(���\�[�X)�֒��_�f�[�^���R�s�[
          std::memcpy(pData, vertices, buffer_->Get().GetDesc().Width);

          // ���_�o�b�t�@(���\�[�X)�ւ̃|�C���^�𖳌��ɂ���
          buffer_->Get().Unmap(
            0,      // �T�u���\�[�X�C���f�b�N�X�ԍ�
            nullptr // �}�b�v�������郁�����͈̔́ACPU���ύX�����\���̂���̈�(nullptr�͑S�̈�)
          );

          // ���_�o�b�t�@�̃r���[������������
          vertex_buffer_view_.BufferLocation = buffer_->Get().GetGPUVirtualAddress(); // �o�b�t�@�̃A�h���X
          vertex_buffer_view_.SizeInBytes = buffer_->Get().GetDesc().Width;           // �o�b�t�@(�S���_���v)�̃T�C�Y(�o�C�g�P��)
          vertex_buffer_view_.StrideInBytes = stride_in_bytes;                        // 1���_�̃T�C�Y(�o�C�g�P��)
        }

        /**
         *  @brief  ���_�o�b�t�@�r���[���擾����
         *  @return ���_�o�b�t�@�r���[
         */
        const D3D12_VERTEX_BUFFER_VIEW& GetView() override
        {
          return vertex_buffer_view_;
        }
        
        /**
         *  @brief  ���_�o�b�t�@���J������
         */
        void Release() noexcept override
        {
          vertex_buffer_view_ = {};
          buffer_.reset();
        }

      private:
        std::unique_ptr<Buffer> buffer_;              ///< �o�b�t�@(���\�[�X)
        D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_; ///< ���_�o�b�t�@�r���[
      };
    };

    /**
     *  @brief  ���_�o�b�t�@���쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
     *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
     */
    std::unique_ptr<IVertexBuffer> IVertexBuffer::Create(ID3D12Device* const device, const std::uint32_t size_in_bytes)
    {
      // �q�[�v�̐ݒ�
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // �q�[�v�̃������v�[��(�s���ɐݒ�)
      properties.CreationNodeMask = 1;                              // ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
      properties.VisibleNodeMask = 1;                               // ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

      auto buffer = new Buffer();
      auto vertex_buffer = std::make_unique<VertexBuffer>();
      vertex_buffer->SetBuffer(buffer);

      try
      {
        buffer->Create(device, properties, size_in_bytes, D3D12_RESOURCE_FLAG_NONE);
      }
      catch (const char*)
      {
        throw std::exception("���_�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B");
      }

      return vertex_buffer;
    }
  };
};