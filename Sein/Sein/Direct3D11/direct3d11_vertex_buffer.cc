/**
 *  @file     vertex_buffer.cc
 *  @brief    Direct3D11�̒��_�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

 // include
#include "direct3d11_vertex_buffer.h"
#include <functional>
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
         *  @param  stride_in_bytes:1���_�̃T�C�Y
         *  @param  vertices:���_�z��ւ̃|�C���^
         */
        void Map(const std::uint32_t stride_in_bytes, const void* const vertices) override
        {
          //D3D11_MAPPED_SUBRESOURCE subresource;
          //deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
          //memcpy(msr.pData, v, sizeof(Vector3) * 3); // 3���_���R�s�[
          //deviceContext->Unmap(vertexBuffer, 0);
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
    std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(ID3D11Device* const device, const std::uint32_t size_in_bytes)
    {
      auto vertex_buffer = std::make_shared<VertexBuffer>();

      return vertex_buffer;
    }
  };
};