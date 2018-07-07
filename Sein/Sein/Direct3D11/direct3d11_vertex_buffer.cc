/**
 *  @file     direct3d11_vertex_buffer.cc
 *  @brief    Direct3D11�̒��_�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

 // include
#include "direct3d11_vertex_buffer.h"
#include <functional>
#include "direct3d11_device_context.h"
#include "direct3d11_buffer.h"

namespace Sein
{
  namespace Direct3D11
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
        VertexBuffer() : buffer_(nullptr)
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~VertexBuffer() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
         */
        void Create(ID3D11Device* const device, const std::uint32_t size_in_bytes)
        {
          D3D11_BUFFER_DESC buffer_desc = {};
          buffer_desc.ByteWidth = size_in_bytes;                ///< �o�b�t�@�̃T�C�Y
          buffer_desc.Usage = D3D11_USAGE_DYNAMIC;              ///< ���I�ɍX�V�\
          buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;     ///< ���_�o�b�t�@
          buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  ///< CPU����A�N�Z�X�\
          buffer_desc.MiscFlags = 0;                            ///< ���̑��̃t���O

          // �o�b�t�@�̍쐬
          buffer_ = IBuffer::Create(device, buffer_desc);
        }

        /**
         *  @brief  �}�b�v����
         *  @param  context:�R���e�L�X�g
         *  @param  size_in_bytes:���_�z��̃T�C�Y
         *  @param  vertices:���_�z��ւ̃|�C���^
         */
        void Map(Renderer::IDeviceContext* const context, const std::uint32_t& size_in_bytes, const void* const vertices) override
        {
          auto& device_context = const_cast<ID3D11DeviceContext&>(static_cast<IDeviceContext*>(context)->GetD3D11DeviceContext());

          D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
          mapped_buffer.pData = nullptr;

          buffer_->Map(&device_context, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

          std::memcpy(mapped_buffer.pData, vertices, size_in_bytes);

          buffer_->Unmap(&device_context, 0);
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
      };
    };

    /**
     *  @brief  ���_�o�b�t�@���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
     *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
     */
    std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(ID3D11Device* const device, const std::uint32_t& size_in_bytes)
    {
      auto vertex_buffer = std::make_shared<VertexBuffer>();

      vertex_buffer->Create(device, size_in_bytes);

      return vertex_buffer;
    }
  };
};