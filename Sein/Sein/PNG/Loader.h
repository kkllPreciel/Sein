/**
 *	@file		Loeader.h
 *	@brief		Png読み込みに関するヘッダファイル
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
		 *	@brief	Png読み込み用interface
		 */
		class ILoader
		{
		public:
			/**
			 *	@brief	デストラクタ
			 */
			virtual ~ILoader() {};

			/**
			 *	@brief	読み込みを行う
			 *	@param	filePath:読み込みを行うPngファイルのパス
			 */
			virtual void Load(std::string filePath) = 0;

			/**
			 *	@brief	データを開放する
			 */
			virtual void Release() = 0;
		};
	};
};