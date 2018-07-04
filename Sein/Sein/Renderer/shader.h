/**
 *  @file     shader.h
 *  @brief    �����_���[�̃V�F�[�_�[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/07/04
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief �V�F�[�_�[�p�C���^�[�t�F�[�X
     */
    class IShader
    {
    public:
      /**
       *  @brief  �V�F�[�_�[���
       */
      enum class Type : std::uint32_t {
        kVertex,    ///< ���_�V�F�[�_�[
        kPixel,     ///< �s�N�Z��(�t���O�����g)�V�F�[�_�[
        kGeometry,  ///< �W�I���g���V�F�[�_�[
        kHull,      ///< �n���V�F�[�_�[
        kDomain,    ///< �h���C���V�F�[�_�[
        kCompute,   ///< �R���s���[�g�V�F�[�_�[
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
       *  @brief  �V�F�[�_�[�̐ݒ���擾����
       */
      virtual const Desc& GetDesc() = 0;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;
    };
  };
};