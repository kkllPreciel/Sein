/**
 *  @file     swap_chain.h
 *  @brief    スワップチェーンに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  スワップチェーン用インターフェイス
     */
    class ISwapChain
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      ISwapChain() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~ISwapChain() = default;

      /**
       *  @brief  現在バックバッファになっているバッファの番号を取得する
       *  @return バックバッファの番号
       */
      virtual std::uint32_t GetCurrentBackBufferIndex() = 0;

      /**
       *  @brief  バックバッファを取得する
       *  @param  buffer_index:取得するバックバッファの番号
       *  @param  resource:バックバッファの格納先
       *  @return エラーコード
       */
      virtual HRESULT GetBuffer(std::uint32_t buffer_index, ID3D12Resource** resource) = 0;

      /**
       *  @brief  レンダリングされたイメージを表示する
       *  @param  sync_interval:垂直同期とフレームの表示を同期する方法
       *  @param  flags:スワップチェーンの表示オプション
       */
      virtual void Present(std::uint32_t sync_interval, std::uint32_t flags) = 0;

      /**
       *  @brief  スワップチェーンを取得する
       *  @return スワップチェーン
       */
      virtual const IDXGISwapChain3& Get() const = 0;

      /**
       *  @brief  スワップチェーンを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      ISwapChain(const ISwapChain& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      ISwapChain& operator = (const ISwapChain& other) = delete;

      /**
       *  @brief  スワップチェーンを作成する
       *  @param  factory:DXGIのファクトリ
       *  @param  command_queue:コマンドキュー
       *  @param  handle:ウィンドウハンドル
       *  @param  swap_chain_desc:スワップチェーンの設定
       *  @return スワップチェーンへのシェアードポインタ
       */
      static std::shared_ptr<ISwapChain> Create(IDXGIFactory4* const factory, ID3D12CommandQueue* const command_queue, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc);
    };
  };
};