/**
 *	@file		Loeader.h
 *	@brief		Png�ǂݍ��݂Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/08/05
 *	@version	1.0
 */

#pragma once

namespace std
{
	class string;
}

namespace Sein
{
	namespace Png
	{
		/**
		 *	@brief	Png�ǂݍ��ݗpinterface
		 */
		class ILoader
		{
		public:
			/**
			 *	@brief	�f�X�g���N�^
			 */
			virtual ~ILoader() {};

			/**
			 *	@brief	�ǂݍ��݂��s��
			 *	@param	filePath:�ǂݍ��݂��s��Png�t�@�C���̃p�X
			 */
			virtual void Load(std::string filePath) = 0;

			/**
			 *	@brief	�f�[�^���J������
			 */
			virtual void Release() = 0;
		};
	};
};