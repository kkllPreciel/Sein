/**
 *  @file     direct3d12_device.h
 *  @brief    Direct3D12�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/02/06
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <vector>
#include <wrl\client.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

namespace Sein
{
  namespace Direct3D12
  {
    class VertexBuffer;
    class IndexBuffer;
    class DepthStencilView;
    class Fence;
    class ConstantBuffer;
    class ShaderResourceBuffer;
    class DescriptorHeap;
    class CommandList;

    /**
     *  @brief  Direct3D12�p�f�o�C�X�N���X(�X���b�v�`�F�C�����ꏏ)
     */
    class Device
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Device();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Device();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      Device(const Device& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      Device& operator = (const Device& other) = delete;

      /**
       *  @brief  �f�o�C�X�𐶐�����
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  width:����
       *  @param  height:�c��
       */
      void Create(HWND handle, unsigned int width, unsigned int height);

      /**
       *  @brief  �f�o�C�X���J������
       */
      void Release();

      /**
       *  @brief  �V�[�����J�n����
       */
      void BeginScene();

      /**
       *  @brief  �V�[�����I������
       */
      void EndScene();

      /**
       *  @brief  ��ʂ��X�V����
       */
      void Present();

      /**
       *  @brief  �萔�o�b�t�@���쐬����
       *  @param  size:�萔�o�b�t�@�̃T�C�Y
       *  @return �萔�o�b�t�@�̃��j�[�N�|�C���^
       */
      std::unique_ptr<ConstantBuffer> CreateConstantBuffer(const unsigned int size);

      /**
       *  @brief  �V�F�[�_�[���\�[�X�o�b�t�@���쐬����
       *  @param  num:���\�[�X���̗v�f��
       *  @param  size:���\�[�X����1�v�f�̃T�C�Y
       *  @return �V�F�[�_�[���\�[�X�o�b�t�@�̃��j�[�N�|�C���^
       */
      std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(const unsigned int num, const unsigned int size);

      /**
       *  @brief  �f�o�C�X���擾����
       *  @return �f�o�C�X�ւ̎Q��
       */
      ID3D12Device& GetDevice() const;

    private:
      ID3D12Device*               device;                                   ///< �f�o�C�X
      IDXGISwapChain3*            swapChain;                                ///< �X���b�v�`�F�C��
      std::unique_ptr<ID3D12CommandQueue, void(*)(IUnknown*)> commandQueue; ///< �R�}���h�L���[
      std::unique_ptr<CommandList> commandList;                             ///< �R�}���h���X�g
      std::unique_ptr<DescriptorHeap[]> descriptorHeaps;                    ///< �f�B�X�N���v�^�[�q�[�v�z��
      std::unique_ptr<Fence> fence;                                         ///< �t�F���X

      // ��X�_�u���o�b�t�@�N���X�ֈړ�
      static const unsigned int FrameCount = 2;               ///< �t���[����(�_�u���o�b�t�@)
      ID3D12Resource*           renderTargetList[FrameCount]; ///< �����_�[�^�[�Q�b�g�̃��X�g
      unsigned int              bufferIndex;                  ///< ���݂̃o�b�t�@�ԍ�

      // �[�x�X�e���V����X�O���ֈړ�
      std::unique_ptr<DepthStencilView> depthStencilView; ///< �[�x�X�e���V���r���[

      /**
       *  @brief  �`��I���҂����s��
       */
      void WaitForGpu();

      /**
       *  @brief  �A�Z�b�g��ǂݍ���
       *  @param  width:�E�B���h�E����
       *  @param  height:�E�B���h�E�c��
       */
      void LoadAssets(unsigned int width, unsigned int height);

      // ���[�g�V�O�l�`���֘A
      // ��X�ʃN���X�ֈړ�
#pragma region RootSignature

      ID3D12RootSignature*  rootSignature;  ///< ���[�g�V�O�l�`��

#pragma endregion

      // �p�C�v���C���X�e�[�g�֘A
      // ��X�ʃN���X�ֈړ�
#pragma region PipelineState
      ID3D12PipelineState*  pipelineState;  ///< �p�C�v���C���X�e�[�g

    public:
      /**
       *  @brief  �`�悷��
       *  @param  vertexBuffer:���_�o�b�t�@
       *  @param  indexBuffer:���_�C���f�b�N�X�o�b�t�@
       *  @param  instanceCount:�C���X�^���X��
       */
      void Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer, const unsigned int instanceCount);
#pragma endregion

      // �e�N�X�`���֘A
      // ��X�ʃN���X�ֈړ�
#pragma region Texture
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> texBuffer;  ///<�e�N�X�`���o�b�t�@

      /**
       *  @brief  �e�N�X�`���o�b�t�@�𐶐�����
       */
      void CreateTextureBuffer();
#pragma endregion
    };
  };
};