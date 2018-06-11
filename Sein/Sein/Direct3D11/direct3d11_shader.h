/**
 *  @file     direct3d11_shader.h
 *  @brief    Direct3D11�̃V�F�[�_�[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/11
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�̃V�F�[�_�[�p�C���^�[�t�F�[�X
     */
    class IShader
    {
    public:
      /**
       *  @brief  �V�F�[�_�[���
       */
      enum class Type : std::uint32_t {
        Vertex,   ///< ���_�V�F�[�_�[
        Pixel,    ///< �s�N�Z��(�t���O�����g)�V�F�[�_�[
        Geometry, ///< �W�I���g���V�F�[�_�[
        Hull,     ///< �n���V�F�[�_�[
        Domain,   ///< �h���C���V�F�[�_�[
        Compute,  ///< �R���s���[�g�V�F�[�_�[
      };

      /**
       *  @brief  �V�F�[�_�[�̐ݒ�
       */
      class Desc
      {
      public:
        const void* shader_bytecode;  ///< �V�F�[�_�[�o�C�g�R�[�h
        std::uint32_t bytecode_size;  ///< �V�F�[�_�[�o�C�g�R�[�h�̃T�C�Y
        Type shader_type;             ///< �V�F�[�_�[���
      };

      /**
       *  @brief  �R���X�g���N�^
       */
      IShader() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IShader() = default;
      
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
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �V�F�[�_�[���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
       *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IShader> Create(ID3D11Device* const device, const Desc& shader_desc);
    };
  };
};