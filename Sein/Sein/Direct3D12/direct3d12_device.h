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
#include "swap_chain.h"
#include "root_signature.h"
#include "command_queue.h"
#include "graphics_pipeline_state.h"
#include "command_list.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"

namespace Sein
{
  namespace Direct3D12
  {
    class DepthStencilView;
    class Fence;
    class ConstantBuffer;
    class ShaderResourceBuffer;
    class DescriptorHeap;
    class TextureView;

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
      void BeginScene(ICommandList* const command_list, std::uint32_t buffer_index);

      /**
       *  @brief  �V�[�����I������
       */
      void EndScene(ICommandList* const command_list, std::uint32_t buffer_index);

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
       *  @brief  �R�}���h���X�g���쐬����
       *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
       *  @return �R�}���h���X�g�̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ICommandList> CreateCommandList(const D3D12_COMMAND_LIST_TYPE& command_list_type);

      /**
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes);

      /**
       *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
       *  @return ���_�C���f�b�N�X�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes);

      /**
       *  @brief  �V�F�[�_�[���쐬����
       *  @param  shader_file_path:�V�F�[�_�[�t�@�C���̃p�X
       *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<IShader> CreateShader(const std::string& shader_file_path);

      /**
       *  @brief  �f�o�C�X���擾����
       *  @return �f�o�C�X�ւ̎Q��
       */
      ID3D12Device& GetDevice() const;

      /**
       *  @brief  ���Ƀo�b�N�o�b�t�@�ɂȂ�o�b�t�@�̔ԍ����擾����
       *  @return ���Ƀo�b�N�o�b�t�@�ɂȂ�o�b�t�@�̔ԍ�
       */
      const std::uint32_t GetNextBackBufferIndex() const;

    private:
      /**
       *  @brief  �X���b�v�`�F�[�����쐬����
       *  @param  factory:DXGI�̃t�@�N�g��
       *  @param  command_queue:�R�}���h�L���[
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  swap_chain_desc:�X���b�v�`�F�[���̐ݒ�
       *  @return �X���b�v�`�F�[���ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ISwapChain> CreateSwapChain(IDXGIFactory4* const factory, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc);

      /**
       *  @brief  �R�}���h�L���[���쐬����
       *  @param  command_queue_desc:�R�}���h�L���[�̐ݒ�
       *  @return �R�}���h�L���[�ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ICommandQueue> CreateCommandQueue(const D3D12_COMMAND_QUEUE_DESC& command_queue_desc);

    private:
      std::unique_ptr<ID3D12Device, void(*)(ID3D12Device*)> device_;  ///< �f�o�C�X
      std::shared_ptr<ICommandQueue> command_queue_;                  ///< �R�}���h�L���[
      std::shared_ptr<ISwapChain> swap_chain_;                        ///< �X���b�v�`�F�[��

      // TODO:�O���ֈړ�����
      std::shared_ptr<IRootSignature> root_signature_;                ///< ���[�g�V�O�l�`��
      std::shared_ptr<IGraphicsPipelineState> pipeline_state_;        ///< �p�C�v���C���X�e�[�g

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

    public:
      /**
       *  @brief  �`�悷��
       *  @param  command_list:�R�}���h���X�g
       *  @param  indexCount:���_�C���f�b�N�X��
       *  @param  instanceCount:�C���X�^���X��
       */
      void Render(ICommandList* const command_list, const unsigned int indexCount, const unsigned int instanceCount);

      // �e�N�X�`���֘A
      // ��X�ʃN���X�ֈړ�
#pragma region Texture
      std::vector<std::unique_ptr<TextureView>> texBuffer;  ///<�e�N�X�`���o�b�t�@

      /**
       *  @brief  �t�@�C������e�N�X�`���o�b�t�@�𐶐�����
       *  @param  file_path:�e�N�X�`���t�@�C���p�X
       */
      void CreateTextureBufferFromFile(const std::wstring& file_path);
#pragma endregion

#pragma region CommandQueue
      /**
       *  @brief  �R�}���h���X�g�����s����
       *  @param  command_list:�R�}���h���X�g
       */
      void ExecuteCommandLists(ICommandList* const command_list);
#pragma endregion
    };
  };
};