/**
 *  @file     renderer.h
 *  @brief    �����_���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/05
 *  @version  1.0
 */

#pragma once

// include
#include <memory>

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  �����_���[�p�C���^�[�t�F�C�X
     */
    class IRenderer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IRenderer() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IRenderer() = default;

      /**
       *  @brief  �����_���[���쐬����
       *  @return �����_���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IRenderer> Create();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IRenderer(const IRenderer& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IRenderer& operator = (const IRenderer& other) = delete;
    };
  };
};