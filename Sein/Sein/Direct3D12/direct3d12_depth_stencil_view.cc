/**
 *  @file     depth_stencil_view.cc
 *  @brief    深度ステンシルビューに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/05/26
 *  @version  1.0
 */

 // include
#include "direct3d12_depth_stencil_view.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コンストラクタ
     */
    DepthStencilView::DepthStencilView() : depthStencil(nullptr, [](IUnknown* p) {p->Release(); })
    {

    }

    /**
     *  @brief  デストラクタ
     */
    DepthStencilView::~DepthStencilView()
    {
      Release();
    }

    /**
     *  @brief  深度ステンシルビューの生成を行う
     *  @param  device:Direct3D12のデバイス
     *  @param  descriptor_handle:ディスクリプターハンドル
     *  @param  width:横幅
     *  @param  height:縦幅
     */
    void DepthStencilView::Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, unsigned int width, unsigned int height)
    {
      Release();

      D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
      depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
      depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
      depthOptimizedClearValue.DepthStencil.Stencil = 0;

      // ヒープの設定
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_DEFAULT;                    // ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
      properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
      properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

      // リソースの設定
      D3D12_RESOURCE_DESC resource_desc;
      resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;   // リソースの種別(今回は2Dテクスチャ)
      resource_desc.Alignment = 0;                                    // アラインメント
      resource_desc.Width = width;                                    // リソースの幅(今回はウィンドウのサイズ)
      resource_desc.Height = height;                                  // リソースの高さ(今回はウィンドウのサイズ)
      resource_desc.DepthOrArraySize = 1;                             // リソースの深さ(テクスチャ等に使用する物、今回は1)
      resource_desc.MipLevels = 0;                                    // ミップマップのレベル(今回は0)
      resource_desc.Format = DXGI_FORMAT_D32_FLOAT;                   // リソースデータフォーマット(R8G8B8A8等)(今回は不明)
      resource_desc.SampleDesc.Count = 1;                             // ピクセル単位のマルチサンプリング数
      resource_desc.SampleDesc.Quality = 0;                           // マルチサンプリングの品質レベル
      resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;            // テクスチャレイアウトオプション
      resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;  // リソース操作オプションフラグ(今回は深度ステンシル)

      ID3D12Resource* resource = nullptr;
      if (FAILED(device->CreateCommittedResource(
        &properties,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        IID_PPV_ARGS(&resource))))
      {
        throw "深度ステンシルビュー用リソースの生成に失敗しました。";
      }
      depthStencil.reset(resource);

      D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
      depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
      depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
      depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

      device->CreateDepthStencilView(depthStencil.get(), &depthStencilDesc, descriptor_handle);
    }

    /**
     *  @brief  リソースを開放する
     */
    void DepthStencilView::Release()
    {
      depthStencil.reset(nullptr);
    }
  };
};