/**
 *  @file     constant_buffer.cc
 *  @brief    �萔�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/04/08
 *  @version  1.0
 */

 // include
#include "constant_buffer.h"
#include "buffer.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �萔�o�b�t�@�p�N���X
       */
      class ConstantBuffer final : public IConstantBuffer
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        ConstantBuffer() : buffer_(nullptr), buffer_pointer_(nullptr), descriptor_handle_({})
        {

        }
        
        /**
         *  @brief  �f�X�g���N�^
         */
        ~ConstantBuffer() override
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
         *  @brief  �f�B�X�N���v�^�[�n���h����ݒ肷��
         *  @param  descriptor_handle:�f�B�X�N���v�^�[�n���h��
         */
        void SetDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle)
        {
          descriptor_handle_ = descriptor_handle;
        }

        /**
         *  @brief  �o�b�t�@�ւ̃|�C���^��ݒ肷��
         *  @param  buffer_pointer:�o�b�t�@�ւ̃|�C���^
         */
        void SetBufferPointer(void* const buffer_pointer)
        {
          buffer_pointer_ = buffer_pointer;
        }
        
        /**
         *  @brief  �}�b�v����
         *  @param  size_in_bytes:�}�b�v����f�[�^�̃T�C�Y
         *  @param  data:�}�b�v����f�[�^�ւ̃|�C���^
         */
        void Map(const std::uint32_t size_in_bytes, const void* const data) override
        {
          if (nullptr == buffer_pointer_)
          {
            throw std::exception("�萔�o�b�t�@�p���\�[�X�ւ̃|�C���^�����݂��܂���B");
          }

          std::memcpy(buffer_pointer_, data, size_in_bytes);
        }
        
        /**
         *  @brief  �萔�o�b�t�@���J������
         */
        void Release() noexcept override
        {
          if (buffer_ && buffer_pointer_)
          {
            buffer_->Get().Unmap(0, nullptr);
          }

          if (buffer_)
          {
            buffer_.reset();
          }

          buffer_pointer_ = nullptr;
          descriptor_handle_ = {};
        }

      private:
        std::unique_ptr<Buffer> buffer_;                ///< �o�b�t�@(���\�[�X)
        void* buffer_pointer_;                          ///< �o�b�t�@�ւ̃|�C���^
        D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle_; ///< �f�B�X�N���v�^�[�n���h��
      };
    };

    /**
     *  @brief  �萔�o�b�t�@���쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  descriptor_handle:�f�B�X�N���v�^�[�n���h��
     *  @param  size_in_bytes:�萔�o�b�t�@�̃T�C�Y
     *  @return �萔�o�b�t�@�ւ̃��j�[�N�|�C���^
     */
    std::unique_ptr<IConstantBuffer> IConstantBuffer::Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const std::uint32_t size_in_bytes)
    {
      // �q�[�v�̐ݒ�
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // �q�[�v�̃������v�[��(�s���ɐݒ�)
      properties.CreationNodeMask = 1;                              // ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
      properties.VisibleNodeMask = 1;                               // ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

      // �萔�o�b�t�@�̃T�C�Y��256�o�C�g�ŃA���C�������g����Ă��Ȃ���΂Ȃ�Ȃ���
      // �����usize�v�Ő��������o�b�t�@�T�C�Y���Ɨ̈悪����Ȃ��\��������B
      // �Ȃ̂ŁA256�̔{���ɕύX���� = (size + 255) & ~255
      std::uint32_t constant_buffer_size = (size_in_bytes + 255) & ~255;

      auto buffer = new Buffer();
      auto constant_buffer = std::make_unique<ConstantBuffer>();
      constant_buffer->SetBuffer(buffer);

      try
      {
        buffer->Create(device, properties, constant_buffer_size, D3D12_RESOURCE_FLAG_NONE);
      }
      catch (const char*)
      {
        throw std::exception("�萔�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B");
      }

      // �萔�o�b�t�@�r���[�̐ݒ�
      D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
      constantBufferViewDesc.BufferLocation = buffer->Get().GetGPUVirtualAddress();   // �o�b�t�@�̃A�h���X
      constantBufferViewDesc.SizeInBytes = constant_buffer_size;                      // �萔�o�b�t�@��256�o�C�g�ŃA���C�������g����Ă��Ȃ���΂Ȃ�Ȃ�

      // �萔�o�b�t�@�r���[�p�̃f�B�X�N���v�^�[�𐶐�
      // �f�B�X�N���v�^�[�q�[�v�̗̈�ɍ쐬�����
      device->CreateConstantBufferView(&constantBufferViewDesc, descriptor_handle);
      constant_buffer->SetDescriptorHandle(descriptor_handle);

      // �}�b�v�BRelease���Ă΂��܂ŃA���}�b�v���Ȃ�
      void* buffer_pointer;
      if (FAILED(buffer->Get().Map(
        0,                                            // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
        nullptr,                                      // CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
        reinterpret_cast<void**>(&buffer_pointer)     // ���\�[�X�f�[�^�ւ̃|�C���^
      )))
      {
        throw std::exception("�萔�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B");
      }

      // ������
      std::memset(buffer_pointer, 0, constant_buffer_size);
      constant_buffer->SetBufferPointer(buffer_pointer);

      return constant_buffer;
    }
  };
};