/**
 *  @file     direct3d11_device.cc
 *  @brief    Direct3D11のデバイスに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

 // include
#include "direct3d11_device.h"
#include <cassert>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  デバイス用クラス
       */
      class Device final : public IDevice
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Device() : device_(nullptr, [](ID3D11Device* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Device() override
        {
          Destroy();
        }

        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          // TODO:デバイスの削除
        }

        /**
         *  @brief  作成する
         *  @param  handle:ウィンドウハンドル
         *  @param  width:ウィンドウの横幅
         *  @param  height:ウィンドウの縦幅
         */
        void Create(const HWND handle, const std::uint32_t width, const std::uint32_t height)
        {
          assert(device_ == nullptr);
        }

      private:
        std::shared_ptr<ID3D11Device> device_;  ///< デバイス
      };
    };

    /**
     *  @brief  デバイスを作成する
     *  @param  handle:ウィンドウハンドル
     *  @param  width:ウィンドウの横幅
     *  @param  height:ウィンドウの縦幅
     */
    std::shared_ptr<IDevice> IDevice::Create(HWND handle, std::uint32_t width, std::uint32_t height)
    {
      auto device = std::make_shared<Device>();
    
      device->Create(handle, width, height);
    
      return device;
    }
  };
};