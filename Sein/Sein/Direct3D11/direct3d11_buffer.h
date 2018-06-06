/**
 *  @file     direct3d11_buffer.h
 *  @brief    Direct3D11のバッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/03
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
     *  @brief  Direct3D11のバッファ用インターフェイス
     */
    class IBuffer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IBuffer() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IBuffer() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IBuffer(const IBuffer& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IBuffer& operator = (const IBuffer& other) = delete;

      /**
       *  @brief  マップする
       *  @param  context:コンテキスト
       *  @param  subresource:サブリソースのインデックス番号
       *  @param  map_type:リソースに対するCPUのアクセス許可設定
       *  @param  map_flags:GPUで使用中だった場合のCPUの対応方法
       *  @param  mapped_resource:マップされたサブリソースへのポインタ
       */
      virtual void Map(ID3D11DeviceContext* const context, UINT subresource, D3D11_MAP map_type, UINT map_flags, D3D11_MAPPED_SUBRESOURCE* const mapped_resource) = 0;

      /**
       *  @brief  アンマップする
       *  @param  context:コンテキスト
       *  @param  subresource:アンマップするサブリソースのインデックス番号
       */
      virtual void Unmap(ID3D11DeviceContext* const context, UINT subresource) = 0;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  バッファを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  buffer_desc:バッファの設定
       *  @return バッファへのシェアードポインタ
       */
      static std::shared_ptr<IBuffer> Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc);
    };
  };
};