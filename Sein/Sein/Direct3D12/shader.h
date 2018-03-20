/**
 *  @file     shader.h
 *  @brief    �V�F�[�_�[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <string>
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �V�F�[�_�[�p�C���^�[�t�F�C�X
     */
    class IShader
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IShader() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IShader() = default;

      /**
       *  @brief  �V�F�[�_�[���擾����
       *  @return �V�F�[�_�[
       */
      virtual const D3D12_SHADER_BYTECODE& Get() const = 0;

      /**
       *  @brief  �V�F�[�_�[���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IShader(const IShader& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IShader& operator = (const IShader& other) = delete;

      /**
       *  @brief  �V�F�[�_�[���쐬����
       *  @param  shader_file_path:�V�F�[�_�[�t�@�C���̃p�X
       *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IShader> Create(const std::string& shader_file_path);
    };
  };
};