/**
 *  @file     command_list.h
 *  @brief    �R�}���h���X�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/08/21
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R�}���h���X�g�pinterface
     */
    class ICommandList
    {
    public:
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ICommandList() = default;

      /**
       *  @brief  �R�}���h���X�g�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  type:�R�}���h���X�g�̃^�C�v
       */
      virtual void Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type) = 0;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() = 0;

      /**
       *  @brief  �L�^���J�n����
       */
      virtual void Begin() = 0;

      /**
       *  @brief  �L�^���I������
       */
      virtual void End() = 0;

      /**
       *  @brief  �R�}���h���X�g���擾����
       *  @return �R�}���h���X�g�̎Q��
       */
      virtual ID3D12GraphicsCommandList& Get() const = 0;
    };

    /**
     *  @brief  �R�}���h���X�g�p�N���X
     */
    class CommandList final : public ICommandList
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      CommandList();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~CommandList() override;

      /**
       *  @brief  �R�}���h���X�g�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  type:�R�}���h���X�g�̃^�C�v
       */
      void Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type) override;

      /**
       *  @brief  ���\�[�X���J������
       */
      void Release() override;

      /**
       *  @brief  �L�^���J�n����
       */
      void Begin() override;

      /**
       *  @brief  �L�^���I������
       */
      void End() override;

      /**
       *  @brief  �R�}���h���X�g���擾����
       *  @return �R�}���h���X�g�̎Q��
       */
      ID3D12GraphicsCommandList& Get() const override;

    private:
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      CommandList(const CommandList& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      CommandList& operator = (const CommandList& other) = delete;

    private:
      std::unique_ptr<ID3D12CommandAllocator, void(*)(IUnknown*)> commandAllocator; ///< �R�}���h�A���P�[�^
      std::unique_ptr<ID3D12GraphicsCommandList, void(*)(IUnknown*)> commandList;   ///< �R�}���h���X�g
    };
  };
};