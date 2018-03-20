/**
 *  @file     swap_chain.cc
 *  @brief    スワップチェーンに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

 // include
#include "swap_chain.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  スワップチェーン用クラス
       */
      class SwapChain final : public ISwapChain
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        SwapChain() : swap_chain_(nullptr, [](IDXGISwapChain3* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~SwapChain() override
        {
          Release();
        }

        /**
         *  @brief  スワップチェーンのを設定する
         *  @param  swap_chain:スワップチェーン
         */
        void SetSwapChain(IDXGISwapChain3* swap_chain)
        {
          swap_chain_.reset(swap_chain);
        }

        /**
         *  @brief  現在バックバッファになっているバッファの番号を取得する
         *  @return バックバッファの番号
         */
        std::uint32_t GetCurrentBackBufferIndex() override
        {
          return swap_chain_->GetCurrentBackBufferIndex();
        }

        /**
         *  @brief  バックバッファを取得する
         *  @param  buffer_index:取得するバックバッファの番号
         *  @param  resource:バックバッファの格納先
         *  @return エラーコード
         */
        HRESULT GetBuffer(std::uint32_t buffer_index, ID3D12Resource** resource) override
        {
          return swap_chain_->GetBuffer(buffer_index, IID_PPV_ARGS(resource));
        }

        /**
         *  @brief  レンダリングされたイメージを表示する
         *  @param  sync_interval:垂直同期とフレームの表示を同期する方法
         *  @param  flags:スワップチェーンの表示オプション
         */
        void Present(std::uint32_t sync_interval, std::uint32_t flags) override
        {
          // 画面の更新
          if (FAILED(swap_chain_->Present(sync_interval, flags)))
          {
            throw std::exception("画面の更新に失敗しました。");
          }
        }
        
        /**
         *  @brief  スワップチェーンを取得する
         *  @return スワップチェーン
         */
        const IDXGISwapChain3& Get() const override
        {
          return *swap_chain_;
        }
        
        /**
         *  @brief  スワップチェーンを開放する
         */
        void Release() noexcept override
        {
          swap_chain_.reset();
        }

      private:
        std::unique_ptr<IDXGISwapChain3, void(*)(IDXGISwapChain3*)> swap_chain_;  ///< スワップチェーン
      };
    };

    /**
     *  @brief  スワップチェーンを作成する
     *  @param  factory:DXGIのファクトリ
     *  @param  command_queue:コマンドキュー
     *  @param  handle:ウィンドウハンドル
     *  @param  swap_chain_desc:スワップチェーンの設定
     *  @return スワップチェーンへのシェアードポインタ
     */
    std::shared_ptr<ISwapChain> ISwapChain::Create(IDXGIFactory4* const factory, ID3D12CommandQueue* const command_queue, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc)
    {
      // TODO:unique_ptrを使用する(例外発生時にreleaseを呼び出さないといけないので)
      IDXGISwapChain1* swap_chain1;
      if (FAILED(factory->CreateSwapChainForHwnd(
        command_queue,    // コマンドキュー
        handle,           // ウィンドウハンドル
        &swap_chain_desc, // スワップチェインの設定情報
        nullptr,          // フルスクリーンスワップチェインの設定(ウィンドウモードで作成するのでnullptr)
        nullptr,          // TODO:調査
        &swap_chain1      // 作成したスワップチェーンの格納先
      )))
      {
        throw std::exception("スワップチェインの生成に失敗しました。");
      }

      IDXGISwapChain3* chain;
      if (FAILED(swap_chain1->QueryInterface(IID_PPV_ARGS(&chain))))
      {
        swap_chain1->Release();
        throw std::exception("IDXGISwapChain3の生成に失敗しました。");
      }
      swap_chain1->Release();

      auto swap_chain = std::make_shared<SwapChain>();
      swap_chain->SetSwapChain(chain);
      return swap_chain;
    }
  };
};