/** *  @file     command_list.cc
 *  @brief    �R�}���h���X�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/08/22
 *  @version  1.0
 */

 // include
#include "command_list.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    CommandList::CommandList() : commandAllocator(nullptr, [](IUnknown* p) {p->Release();}), commandList(nullptr, [](IUnknown* p) {p->Release(); })
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    CommandList::~CommandList()
    {
      Release();
    }
    
    /**
     *  @brief  �R�}���h���X�g�𐶐�����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  type:�R�}���h���X�g�̃^�C�v
     */
    void CommandList::Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type)
    {
      Release();

      ID3D12CommandAllocator* allocator;
      if (FAILED(device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator))))
      {
        throw "�R�}���h�A���P�[�^�[�̐����Ɏ��s���܂����B";
      }
      commandAllocator.reset(allocator);

      ID3D12GraphicsCommandList* list;
      if (FAILED(device->CreateCommandList(
        0,                            // �}���`�A�_�v�^�[(�}���`GPU)�̏ꍇ�Ɏg�p����A�_�v�^�[(GPU)�̎��ʎq(�P��Ȃ̂�0)
        type,                         // �R�}���h���X�g�̎��(�����_�����O�֘A�̃R�}���h���X�g)
        allocator,                    // ���̃R�}���h���X�g�Ŏg�p����R�}���h�A���P�[�^
        nullptr,                      // �R�}���h���X�g�̏����p�C�v���C�����(�_�~�[�̏����p�C�v���C�����w��)
        IID_PPV_ARGS(&list))))
      {
        throw "�R�}���h���X�g�̐����Ɏ��s���܂����B";
      }
      commandList.reset(list);

      End();
    }
    
    /**
     *  @brief  ���\�[�X���J������
     */
    void CommandList::Release()
    {
      commandList.reset(nullptr);
      commandAllocator.reset(nullptr);
    }
    
    /**
     *  @brief  �L�^���J�n����
     */
    void CommandList::Begin()
    {
      if (FAILED(commandAllocator->Reset()))
      {
        throw "�R�}���h�A���P�[�^�[�̃��Z�b�g�Ɏ��s���܂����B";
      }

      if (FAILED(commandList->Reset(commandAllocator.get(), nullptr)))
      {
        throw "�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂����B";
      }
    }
    
    /**
     *  @brief  �L�^���I������
     */
    void CommandList::End()
    {
      commandList->Close();
    }

    /**
     *  @brief  �R�}���h���X�g���擾����
     *  @return �R�}���h���X�g�̎Q��
     */
    ID3D12GraphicsCommandList& CommandList::Get() const
    {
      return *commandList;
    }
  };
};