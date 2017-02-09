/**
 *	@file		Direct3D12Device.cpp
 *	@brief		Direct3D12デバイスに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/02/07
 *	@version	1.0
 */

#if _DEBUG
#include "Debugger.h"
#include <Windows.h>
#include <sstream>
#endif
#include "Direct3D12Device.h"

namespace Sein
{
	namespace Direct3D12
	{
		/** 
		 *	@brief	コンストラクタ
		 */
		Device::Device() : device(nullptr), swapChain(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		Device::~Device()
		{
			Release();
		}

		/**
		 *	@brief	デバイスを生成する
		 */
		void Device::Create()
		{
#if _DEBUG
			// デバッグレイヤーを有効に設定する
			Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
			{
				throw "デバッグレイヤーの生成に失敗しました。";	
			}
			debugInterface->EnableDebugLayer();
#endif
			// ファクトリの生成
			// アダプターの列挙に使用
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

			if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
			{
				throw "DXGIファクトリの生成に失敗しました。";
			}

			// Direct3D12のデバイスを作成する
			Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

			// アダプター(ビデオカード)を列挙する
			for (auto i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
			{
				DXGI_ADAPTER_DESC1 desc;
				pAdapter->GetDesc1(&desc);

				// TODO:調査
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					continue;
				}

				// デバイスを作成
				if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
				{
					continue;
				}

#if _DEBUG
				std::wostringstream ostr;
				pAdapter->GetDesc1(&desc);
				ostr << "D3D12-capable hardware found:" << desc.Description << "(" << (desc.DedicatedVideoMemory >> 20) << "MB)\n";
				OutputDebugString(ostr.str().c_str());
#endif
			}
			
			// WARPで生成する
			if (nullptr == device)
			{
#if _DEBUG
				OutputDebugString(L"WARP software adapter requested.  Initializing...\n");
#endif
				if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter))))
				{
					throw "WARPアダプターが存在しません。";
				}

				if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
				{
					throw "デバイスの生成に失敗しました。";
				}
			}

			// コマンドキューの作成
			// コマンドキューはGPUへ描画命令が出されたコマンドリストを
			// 順次実行する
			// つまり特定のアダプター(GPU)に関連付けられている
			{

			}

			// スワップチェインの作成
			// コマンドキューを指定して作成する = アダプターを指定して作成する
			{

			}

			// 描画管理
			// スワップチェイン生成

			// 描画コマンド関連
			// コマンドキュー生成
			// コマンドアロケータ生成

			// 描画対象・管理
			// フェンス生成
			// ディスクリプターヒープの作成
			// レンダーターゲットビューの作成
		}

		/**
		 *	@brief	デバイスを開放する
		 */
		void Device::Release()
		{

		}

		/**
		 *	@brief	画面を更新する
		 */
		void Device::Present()
		{

		}
	};
};