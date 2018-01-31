/**
 *  @file     direct3d12_device.cc
 *  @brief    Direct3D12�f�o�C�X�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/02/07
 *  @version  1.0
 */

#if _DEBUG
#include <sstream>
#endif
#include <winerror.h>
#include <d3dcompiler.h>
#include "direct3d12_device.h"
#include "depth_stencil_view.h"
#include "fence.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "constant_buffer.h"
#include "shader_resource_buffer.h"
#include "descriptor_heap.h"
#include "descriptor.h"
#include "command_list.h"
#include "buffer.h"
#include "texture.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    Device::Device() :
      device(nullptr, [](IUnknown* p) { p->Release(); }), swapChain(nullptr, [](IUnknown* p) { p->Release(); }), commandQueue(nullptr, [](IUnknown* p) { p->Release(); }), commandList(nullptr),
      descriptorHeaps(nullptr), bufferIndex(0), rootSignature(nullptr), pipelineState(nullptr),
      depthStencilView(nullptr), fence(nullptr), texBuffer(nullptr)
    {
      for (auto i = 0; i < FrameCount; ++i)
      {
        renderTargetList[i] = nullptr;
      }
    }

    /**
     *  @brief  �f�X�g���N�^
     */
    Device::~Device()
    {
      Release();
    }

    /**
     *  @brief  �f�o�C�X�𐶐�����
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  width:����
     *  @param  height:�c��
     */
    void Device::Create(HWND handle, unsigned int width, unsigned int height)
    {
#if _DEBUG
      // �f�o�b�O���C���[��L���ɐݒ肷��
      Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
      if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
      {
        throw "�f�o�b�O���C���[�̐����Ɏ��s���܂����B";
      }
      debugInterface->EnableDebugLayer();
#endif
      // �t�@�N�g���̐���
      // �A�_�v�^�[�̗񋓂Ɏg�p
      Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

      if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
      {
        throw "DXGI�t�@�N�g���̐����Ɏ��s���܂����B";
      }

      // Direct3D12�̃f�o�C�X���쐬����
      {
#if 1
        // �f�o�C�X�𐶐�
        // �A�_�v�^�[��񋓂��쐬�����
        // ���ɂ���ă��������[�N��������������
        // �f�t�H���g�̃A�_�v�^�[���g�p���쐬����
        ID3D12Device* pDevice;
        if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice))))
        {
          throw "�f�o�C�X�̐����Ɏ��s���܂����B";
        }
        device.reset(pDevice);
#else
        Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

        // �A�_�v�^�[(�r�f�I�J�[�h)��񋓂���
        for (auto i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
        {
          DXGI_ADAPTER_DESC1 desc;
          pAdapter->GetDesc1(&desc);

          // TODO:����
          if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
          {
            continue;
          }

          // �f�o�C�X���쐬
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

        // WARP�Ő�������
        if (nullptr == device)
        {
#if _DEBUG
          OutputDebugString(L"WARP software adapter requested.  Initializing...\n");
#endif
          if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter))))
          {
            throw "WARP�A�_�v�^�[�����݂��܂���B";
          }

          if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
          {
            throw "�f�o�C�X�̐����Ɏ��s���܂����B";
          }
        }
#endif
      }

      // �R�}���h�L���[�̍쐬
      // �R�}���h�L���[��GPU�֕`�施�߂��o���ꂽ�R�}���h���X�g��
      // �������s����
      // �܂����̃A�_�v�^�[(GPU)�Ɋ֘A�t�����Ă���
      {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // �^�C���A�E�g������L���ɂ���
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;  // �����_�����O�֘A�̃R�}���h���X�g

        ID3D12CommandQueue* queue;
        if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue))))
        {
          throw "�R�}���h�L���[�̐����Ɏ��s���܂����B";
        }
        commandQueue.reset(queue);
      }

      // �X���b�v�`�F�C���̍쐬
      // �R�}���h�L���[���w�肵�č쐬���� = �A�_�v�^�[���w�肵�č쐬����
      {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = width;                                  // �E�B���h�E����
        swapChainDesc.Height = height;                                // �E�B���h�E�c��
        swapChainDesc.BufferCount = FrameCount;                       // �o�b�t�@�̐���2��(�t�����g�o�b�t�@���܂ނ炵�����A�����T���v����������ƃo�b�N�o�b�t�@�̐��ȋC������)
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // ���炭�o�b�t�@�̃t�H�[�}�b�g(4 �����A32 �r�b�g�����Ȃ�����)
        swapChainDesc.Scaling = DXGI_SCALING_NONE;                    // ��ʃT�C�Y�ƃo�b�t�@�T�C�Y���������Ȃ����̊g�k����(�g��k���͍s��Ȃ�)
        swapChainDesc.SampleDesc.Quality = 0;                         // �}���`�T���v�����O�̕i�����x��
        swapChainDesc.SampleDesc.Count = 1;                           // �s�N�Z���P�ʂ̃}���`�T���v�����O��
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // �o�b�N�o�b�t�@�̎g�p�ړI�y��CPU�A�N�Z�X�I�v�V����(�����_�[�^�[�Q�b�g�Ƃ��Ďg�p)
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �X���b�v�`�F�C���̓���I�v�V����(���[�h�ؑ։\�ɐݒ�)
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̃X���b�v�����w��(�o�b�N�o�b�t�@���f�B�X�v���C�ɕ\�����ꂽ��j������)

        Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain;
        if (FAILED(factory->CreateSwapChainForHwnd(
          commandQueue.get(), // �R�}���h�L���[
          handle,             // �E�B���h�E�n���h��
          &swapChainDesc,     // �X���b�v�`�F�C���̐ݒ���
          nullptr,            // �t���X�N���[���X���b�v�`�F�C���̐ݒ�(�E�B���h�E���[�h�ō쐬����̂�nullptr)
          nullptr,            // TODO:����
          &pSwapChain)))
        {
          throw "�X���b�v�`�F�C���̐����Ɏ��s���܂����B";
        }

        IDXGISwapChain3* chain;
        if (FAILED(pSwapChain.Get()->QueryInterface(IID_PPV_ARGS(&chain))))
        {
          throw "IDXGISwapChain3�̐����Ɏ��s���܂����B";
        }
        swapChain.reset(chain);

        // �o�b�N�o�b�t�@�̔ԍ����擾����
        bufferIndex = swapChain->GetCurrentBackBufferIndex();
      }

      // �R�}���h���X�g�̐���
      commandList = std::make_unique<CommandList>();
      commandList->Create(device.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);

      // Alt + Enter�Ńt���X�N���[�����̋@�\�𖳌��ɐݒ�
      factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);

      // �f�B�X�N���v�^�[�q�[�v�p�������̈�̊m��
      {
        descriptorHeaps = std::make_unique<DescriptorHeap[]>(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES);
      }

      // �萔�o�b�t�@�r���[�A�V�F�[�_�[���\�[�X�r���[�p�f�B�X�N���v�^�[�q�[�v�𐶐�
      {
        auto& cbvSrvHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
        cbvHeapDesc.NumDescriptors = 5;                                 // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�萔�o�b�t�@�A�V�F�[�_�[���\�[�X)
        cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // �萔�o�b�t�@ or �V�F�[�_�[���\�[�X(�e�N�X�`��) or �����_���A�N�Z�X �̂ǂꂩ�̃q�[�v
        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // �V�F�[�_�[����A�N�Z�X��
        cbvSrvHeap.Create(device.get(), cbvHeapDesc);
      }

      // �[�x�X�e���V���r���[�p�f�B�X�N���v�^�[�q�[�v�𐶐�
      {
        auto& dsvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = 1;                      // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�[�x�X�e���V���r���[)
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;   // �[�x�X�e���V���r���[�̃q�[�v
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �V�F�[�_�[����A�N�Z�X�s��
        dsvDescriptorHeap.Create(device.get(), heapDesc);
      }

      // �����_�[�^�[�Q�b�g
      {
        auto& rtvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];

        // �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�[�q�[�v�̍쐬
        // �f�B�X�N���v�^�[�̓o�b�t�@�̏��f�[�^(�e�N�X�`���o�b�t�@�A���_�o�b�t�@��)
        {
          D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
          rtvHeapDesc.NumDescriptors = FrameCount;              // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�t�����g�o�b�t�@�A�o�b�N�o�b�t�@)
          rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;    // ��ʂ̓����_�[�^�[�Q�b�g�r���[
          rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // �V�F�[�_�[����Q�Ƃ��Ȃ�
          rtvDescriptorHeap.Create(device.get(), rtvHeapDesc);
        }

        // �f�B�X�N���v�^�[�̓o�^
        {
          // �t���[���o�b�t�@�����o�^����
          for (auto i = 0; i < FrameCount; ++i)
          {
            if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargetList[i]))))
            {
              throw "�o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B";
            }

            const auto& descriptor = rtvDescriptorHeap.CreateDescriptor();

            // �����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�[���쐬����
            // �f�B�X�N���v�^�[�q�[�v�̗̈�ɍ쐬�����
            device->CreateRenderTargetView(
              renderTargetList[i],  // �����_�[ �^�[�Q�b�g��\��ID3D12Resource�ւ̃|�C���^
              nullptr,              // D3D12_RENDER_TARGET_VIEW_DESC�ւ̃|�C���^
              descriptor.GetHandleForCPU()
            );
          }
        }
      }

      // �t�F���X�̐���
      // Directx12�ł�GPU�̕`��̏I���҂��������ōs��Ȃ�(��������ꂸ��ʂ����������Ȃ�)
      // ���̂��ߓ�������邽�߂̃I�u�W�F�N�g(�t�F���X)���쐬����
      {
        fence = std::make_unique<Fence>();
        fence->Create(device.get());

        // �`�揈�����s���Ă���\��������̂ŕ`��I���҂����s��
        WaitForGpu();
      }

      // �A�Z�b�g��ǂݍ���
      {
        LoadAssets(width, height);
      }
    }

    /**
     *	@brief	�f�o�C�X���J������
     */
    void Device::Release()
    {
      // GPU�̕`��I���҂����s��
      WaitForGpu();

      pipelineState->Release();
      rootSignature->Release();

      for (auto i = 0; i < FrameCount; ++i)
      {
        renderTargetList[i]->Release();
      }
    }

    /**
     *	@brief	�V�[�����J�n����
     */
    void Device::BeginScene()
    {
      commandList->Begin();

      // �o�b�N�o�b�t�@���`��^�[�Q�b�g�Ƃ��Ďg�p�ł���悤�ɂȂ�܂ő҂�
      D3D12_RESOURCE_BARRIER barrier;
      barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// �o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
      barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
      barrier.Transition.pResource = renderTargetList[bufferIndex];			// ���\�[�X�͕`��^�[�Q�b�g
      barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;			// �J�ڑO��Present
      barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		// �J�ڌ�͕`��^�[�Q�b�g
      barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      commandList->Get().ResourceBarrier(1, &barrier);

      // �o�b�N�o�b�t�@��`��^�[�Q�b�g�Ƃ��Đݒ肷��
      // �f�o�C�X�֐[�x�X�e���V���r���[���o�C���h����
      auto& rtvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];
      const auto& rtvDescriptor = rtvDescriptorHeap.GetDescriptor(bufferIndex);
      auto& dsvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
      const auto& dsvDescriptor = dsvDescriptorHeap.GetDescriptor(0); // TODO:�}�W�b�N�i���o�[������
      commandList->Get().OMSetRenderTargets(1, &rtvDescriptor.GetHandleForCPU(), false, &dsvDescriptor.GetHandleForCPU());

      // �o�b�N�o�b�t�@���N���A����
      const float Color[] = { 0.0f, 0.0f, 0.6f, 1.0f };
      commandList->Get().ClearRenderTargetView(rtvDescriptor.GetHandleForCPU(), Color, 0, nullptr);

      // �[�x�X�e���V���r���[���N���A����(�[�x�o�b�t�@�̂�)
      commandList->Get().ClearDepthStencilView(dsvDescriptor.GetHandleForCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    }

    /**
     *	@brief	�V�[�����I������
     */
    void Device::EndScene()
    {
      // �o�b�N�o�b�t�@�̕`�抮����҂��߂̃o���A��ݒu
      D3D12_RESOURCE_BARRIER barrier;
      barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// �o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
      barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
      barrier.Transition.pResource = renderTargetList[bufferIndex];			// ���\�[�X�͕`��^�[�Q�b�g
      barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �J�ڑO�͕`��^�[�Q�b�g
      barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �J�ڌ��Present
      barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      commandList->Get().ResourceBarrier(1, &barrier);

      // �R�}���h���X�g���N���[�Y����
      commandList->End();
    }

    /**
     *	@brief	��ʂ��X�V����
     */
    void Device::Present()
    {
      // �R�}���h���X�g�̎��s
      ID3D12CommandList* ppCommandLists[] = { &(commandList->Get()) };
      commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

      // �`��I���҂����s��
      WaitForGpu();

      // ��ʂ̍X�V
      if (FAILED(swapChain->Present(1, 0)))
      {
        throw "��ʂ̍X�V�Ɏ��s���܂����B";
      }

      // �o�b�t�@�ԍ����X�V
      bufferIndex = swapChain->GetCurrentBackBufferIndex();
    }

    /**
     *	@brief	�`��I���҂����s��
     */
    void Device::WaitForGpu()
    {
      fence->Wait(commandQueue.get());
    }

    /**
     *	@brief	�A�Z�b�g��ǂݍ���
     *	@param	width:�E�B���h�E����
     *	@param	height:�E�B���h�E�c��
     */
    void Device::LoadAssets(unsigned int width, unsigned int height)
    {
      // �[�x�X�e���V���r���[�̍쐬
      {
        depthStencilView = std::make_unique<DepthStencilView>();
        auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
        auto& descriptor = descriptorHeap.CreateDescriptor();
        depthStencilView->Create(device.get(), descriptor.GetHandleForCPU(), width, height);
      }

      // ���[�g�V�O�l�`���̍쐬
      {
        // �f�B�X�N���v�^�[�����W�̐ݒ�(�萔�o�b�t�@�ƃV�F�[�_�[���\�[�X(���_�V�F�[�_�[�݂̂Ŏg�p��)�ƃV�F�[�_�[���\�[�X(�s�N�Z���V�F�[�_�[�݂̂Ŏg�p��))
        D3D12_DESCRIPTOR_RANGE descriptorRanges[3];
        {
          // �萔�o�b�t�@�p�f�B�X�N���v�^�[�����W�̐ݒ�
          descriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;								// �f�B�X�N���v�^�[�̎��(�萔�o�b�t�@�r���[)
          descriptorRanges[0].NumDescriptors = 1;															// �f�B�X�N���v�^�[�̐�
          descriptorRanges[0].BaseShaderRegister = 0;														// �͈͓��̃x�[�X�V�F�[�_���W�X�^
          descriptorRanges[0].RegisterSpace = 0;															// ���W�X�^���(TODO:���ׂ�)
          descriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// ���[�g�V�O�l�`���J�n����̃f�B�X�N���v�^�̃I�t�Z�b�g?

          // StructuredBuffer�p�f�B�X�N���v�^�[�����W�̐ݒ�
          descriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;								// �f�B�X�N���v�^�[�̎��(�V�F�[�_�[���\�[�X�r���[)
          descriptorRanges[1].NumDescriptors = 1;															// �f�B�X�N���v�^�[�̐�
          descriptorRanges[1].BaseShaderRegister = 0;														// �͈͓��̃x�[�X�V�F�[�_���W�X�^
          descriptorRanges[1].RegisterSpace = 0;															// ���W�X�^���(TODO:���ׂ�)
          descriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// ���[�g�V�O�l�`���J�n����̃f�B�X�N���v�^�̃I�t�Z�b�g?

          // �e�N�X�`���p�f�B�X�N���v�^�[�����W�̐ݒ�
          descriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;                              // �f�B�X�N���v�^�[�̎��(�V�F�[�_�[���\�[�X�r���[)
          descriptorRanges[2].NumDescriptors = 3;                                                       // �f�B�X�N���v�^�[�̐�
          descriptorRanges[2].BaseShaderRegister = 0;                                                   // �͈͓��̃x�[�X�V�F�[�_���W�X�^
          descriptorRanges[2].RegisterSpace = 0;                                                        // ���W�X�^���(TODO:���ׂ�)
          descriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // ���[�g�V�O�l�`���J�n����̃f�B�X�N���v�^�̃I�t�Z�b�g?
        }

        // ���[�g�p�����[�^�̐ݒ�
        D3D12_ROOT_PARAMETER rootParameters[3];
        {
          // �萔�o�b�t�@�p���[�g�p�����[�^
          rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ���[�g�V�O�l�`���̃X���b�g�̎��(�f�B�X�N���v�^�e�[�u��)
          rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;									// �f�B�X�N���v�^�[�����W�̐�
          rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRanges[0];					// �f�B�X�N���v�^�[�����W�̃|�C���^(����1���Ȃ�z��̐擪�|�C���^)
          rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;						// ���[�g�V�O�l�`���̃X���b�g�̓��e�ɃA�N�Z�X�ł���V�F�[�_�̎��(���_�V�F�[�_�̂�)

          // StructuredBuffer�p���[�g�p�����[�^
          rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ���[�g�V�O�l�`���̃X���b�g�̎��(�f�B�X�N���v�^�e�[�u��)
          rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;									// �f�B�X�N���v�^�[�����W�̐�
          rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRanges[1];					// �f�B�X�N���v�^�[�����W�̃|�C���^(����1���Ȃ�z��̐擪�|�C���^)
          rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;						// ���[�g�V�O�l�`���̃X���b�g�̓��e�ɃA�N�Z�X�ł���V�F�[�_�̎��(���_�V�F�[�_�̂�)

          // �e�N�X�`���p���[�g�p�����[�^
          rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ���[�g�V�O�l�`���̃X���b�g�̎��(�f�B�X�N���v�^�e�[�u��)
          rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;									// �f�B�X�N���v�^�[�����W�̐�
          rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRanges[2];					// �f�B�X�N���v�^�[�����W�̃|�C���^(����1���Ȃ�z��̐擪�|�C���^)
          rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;							// ���[�g�V�O�l�`���̃X���b�g�̓��e�ɃA�N�Z�X�ł���V�F�[�_�̎��(�s�N�Z���V�F�[�_�̂�)
        }

        // �T���v���[�̐ݒ�
        D3D12_STATIC_SAMPLER_DESC sampler = {};
        sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;					// �t�B���^�����O���@(�|�C���g�T���v�����O)
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// �͈͊O�ɂ���U���W�̉������@(���E���̐F���g�p)
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// �͈͊O�ɂ���V���W�̉������@(���E���̐F���g�p)
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// �͈͊O�ɂ���W���W�̉������@(���E���̐F���g�p)
        sampler.MipLODBias = 0;												// �~�b�v�}�b�v ���x������̃I�t�Z�b�g
        sampler.MaxAnisotropy = 0;											// D3D12_FILTER_ANISOTROPIC��D3D12_FILTER_COMPARISON_ANISOTROPIC���t�B���^�Ƃ��Ďw�肵���ꍇ�̃N�����v�l
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;				// �����̃T���v�����O�f�[�^�ɑ΂��ăf�[�^���r����֐�(��r���Ȃ�)
        sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �͈͊O���W�̉������@�ɑ΂���D3D12_TEXTURE_ADDRESS_MODE_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F
        sampler.MinLOD = 0.0f;												// �N�����v����~�b�v�}�b�v�͈͂̉���
        sampler.MaxLOD = D3D12_FLOAT32_MAX;									// �N�����v����~�b�v�}�b�v�͈͂̏��(������Ȃ��ꍇ��D3D12_FLOAT32_MAX���̑傫���T�C�Y���w�肷��)
        sampler.ShaderRegister = 0;											// ���W�X�^�[�ԍ�(�uregister(s2, space3)�v��s2�̔ԍ��w��p)
        sampler.RegisterSpace = 0;											// ���W�X�^�[�X�y�[�X(�uregister(s2, space3)�v��space3�̔ԍ��w��p)
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_�[����A�N�Z�X�\

        // ���[�g�V�O�l�`���̐ݒ�
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.NumParameters = 3;													// ���[�g�V�O�l�`���̃X���b�g��
        rootSignatureDesc.pParameters = rootParameters;											// �X���b�g�̍\��?
        rootSignatureDesc.NumStaticSamplers = 1;												// �ÓI�T���v���[��
        rootSignatureDesc.pStaticSamplers = &sampler;											// �ÓI�T���v���[�ݒ�f�[�^�̃|�C���^
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT	// �I�v�V����(�`��Ɏg�p����)
          | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS							// �n���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
          | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS							// �h���C���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
          | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;						// �W�I���g���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~


        // ���[�g�V�O�l�`���̃V���A����
        Microsoft::WRL::ComPtr<ID3DBlob> signature;
        if (FAILED(D3D12SerializeRootSignature(
          &rootSignatureDesc,				// ���[�g�V�O�l�`���̐ݒ�
          D3D_ROOT_SIGNATURE_VERSION_1,	// ���[�g�V�O�l�`���̃o�[�W����
          &signature,						// �V���A���C�Y�������[�g�V�O�l�`���փA�N�Z�X���邽�߂̃C���^�[�t�F�C�X(�|�C���^)
          nullptr							// �V���A���C�U�̃G���[���b�Z�[�W�փA�N�Z�X���邽�߂̃C���^�[�t�F�C�X(�|�C���^)
        )))
        {
          throw "���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂����B";
        }

        // ���[�g�V�O�l�`���̐���
        if (FAILED(device->CreateRootSignature(
          0,									// �}���`�A�_�v�^�[(�}���`GPU)�̏ꍇ�Ɏg�p����A�_�v�^�[(GPU)�̎��ʎq(�P��Ȃ̂�0)
          signature->GetBufferPointer(),		// �V���A�������ꂽ�V�O�l�`���ݒ�ւ̃|�C���^
          signature->GetBufferSize(),			// �������̃u���b�N�T�C�Y
          IID_PPV_ARGS(&rootSignature))))
        {
          throw "���[�g�V�O�l�`���̐����Ɏ��s���܂����B";
        }
      }

      // �p�C�v���C���X�e�[�g�̍쐬
      // �V�F�[�_�[���ꏏ�ɃR���p�C������
      // ��X�̓R���p�C���ς݂̃V�F�[�_�[���g�p����
      {
        Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

        // �R���p�C���I�v�V�����t���O��ݒ肷��
#if defined(_DEBUG)
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        // ���_�V�F�[�_�[�̃R���p�C��
        if (FAILED(D3DCompileFromFile(
          L"shaders.hlsl",	// �V�F�[�_�[�t�@�C����
          nullptr,			// �V�F�[�_�[�}�N��(����͎g�p���Ȃ�)
          nullptr,			// �C���N���[�h�t�@�C������舵�����߂Ɏg�p����ID3DInclude�C���^�t�F�[�X�ւ̃|�C���^(����͎g�p���Ȃ�)
          "VSMain",			// �G���g���[�|�C���g�̊֐���
          "vs_5_0",			// �R���p�C���^�[�Q�b�g(����͒��_�V�F�[�_�[�ŃV�F�[�_�[���f��5)
          compileFlags,		// �R���p�C���I�v�V����
          0,					// �G�t�F�N�g�t�@�C���̃R���p�C���I�v�V����(����̓G�t�F�N�g�Ƃ��Ďg�p���Ȃ��̂�0)
          &vertexShader,		// �R���p�C�����ꂽ�R�[�h�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
          nullptr				// �R���p�C���G���[���b�Z�[�W�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
        )))
        {
          throw "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂����B";
        }

        // �s�N�Z���V�F�[�_�[�̃R���p�C��
        if (FAILED(D3DCompileFromFile(
          L"shaders.hlsl",	// �V�F�[�_�[�t�@�C����
          nullptr,			// �V�F�[�_�[�}�N��(����͎g�p���Ȃ�)
          nullptr,			// �C���N���[�h�t�@�C������舵�����߂Ɏg�p����ID3DInclude�C���^�t�F�[�X�ւ̃|�C���^(����͎g�p���Ȃ�)
          "PSMain",			// �G���g���[�|�C���g�̊֐���
          "ps_5_0",			// �R���p�C���^�[�Q�b�g(����̓s�N�Z���V�F�[�_�[�ŃV�F�[�_�[���f��5)
          compileFlags,		// �R���p�C���I�v�V����
          0,					// �G�t�F�N�g�t�@�C���̃R���p�C���I�v�V����(����̓G�t�F�N�g�Ƃ��Ďg�p���Ȃ��̂�0)
          &pixelShader,		// �R���p�C�����ꂽ�R�[�h�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
          nullptr				// �R���p�C���G���[���b�Z�[�W�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
        )))
        {
          throw "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂����B";
        }

        // ���_���̓��C�A�E�g
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
          // ���_�f�[�^
          { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
          { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
          { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        // ���X�^���C�U�[�X�e�[�g�̐ݒ�
        D3D12_RASTERIZER_DESC rasterizer_desc;
        rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;                               // �O�p�`��`�悷��Ƃ��Ɏg�p����h��Ԃ����[�h(����͓h��Ԃ�)
        rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;                                // �J�����O�̃��[�h(�������̃|���S����`�悵�Ȃ�)
        rasterizer_desc.FrontCounterClockwise = FALSE;                                  // �|���S���̕\���̔�����@(����͎��v���Ȃ�\)
        rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // �[�x�o�C�A�X(Z-Fighting�΍�?)
        rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // �[�x�o�C�A�X�̃N�����v�l
        rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // �X�΂��l�������[�x�o�C�A�X(�s�[�^�[�p�����ۑ΍�?)
        rasterizer_desc.DepthClipEnable = TRUE;                                         // Z�N���b�s���O���s����(����͍s��)
        rasterizer_desc.MultisampleEnable = FALSE;                                      // �}���`�T���v�����O�̃A���`�G�C���A�V���O���s����(����͂��Ȃ�)
        rasterizer_desc.AntialiasedLineEnable = FALSE;                                  // ���̃A���`�G�C���A�V���O���s����(����͂��Ȃ�)
        rasterizer_desc.ForcedSampleCount = 0;                                          // UAV�����_�����O�܂��̓��X�^���C�Y���ɋ��������T���v����(����͋������Ȃ�)
        rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // �ێ�I�ȃ��X�^���C�Y�t���O(����̓I�t)

        // �p�C�v���C���X�e�[�g�̐ݒ�Ɏg�p���钸�_�V�F�[�_�[�̃f�[�^�\�����쐬
        D3D12_SHADER_BYTECODE vs;
        vs.pShaderBytecode = vertexShader.Get()->GetBufferPointer();
        vs.BytecodeLength = vertexShader.Get()->GetBufferSize();

        // �p�C�v���C���X�e�[�g�̐ݒ�Ɏg�p����s�N�Z���V�F�[�_�[�̃f�[�^�\�����쐬
        D3D12_SHADER_BYTECODE ps;
        ps.pShaderBytecode = pixelShader.Get()->GetBufferPointer();
        ps.BytecodeLength = pixelShader.Get()->GetBufferSize();

        // �����_�[�^�[�Q�b�g�̃u�����h��Ԃ̐ݒ�
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTarggetBlendDesc =
        {
          false,                        // �u�����f�B���O��L���ɂ��邩(����͖���)
          false,                        // �_�����Z��L���ɂ��邩(����͖���)
          D3D12_BLEND_ONE,              // RGB�l�s�N�Z���V�F�[�_�o�͂ɑ΂��Ď��s���鑀����w��
          D3D12_BLEND_ZERO,             // �����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���铮����w�肷��
          D3D12_BLEND_OP_ADD,           // RGB�܂��̓A���t�@�u�����f�B���O������w��(�\�[�X1�ƃ\�[�X2��ǉ�)
          D3D12_BLEND_ONE,              // �s�N�Z���V�F�[�_�o�͂���A���t�@�l�ɑ΂��Ď��s���铮����w��
          D3D12_BLEND_ZERO,             // �����_�[�^�[�Q�b�g�Ō��݂̃A���t�@�l�ɑ΂��Ď��s���铮��
          D3D12_BLEND_OP_ADD,           // RBG�܂��̓A���t�@�u�����f�B���O������w��(�\�[�X1�ƃ\�[�X2��ǉ�)
          D3D12_LOGIC_OP_NOOP,          // �����_�[�^�[�Q�b�g�ɐݒ肷��_�����Z?(�m�[�I�y���[�V�����A�������Ȃ�)
          D3D12_COLOR_WRITE_ENABLE_ALL  // 
        };

        // �u�����h��Ԃ̐ݒ�
        D3D12_BLEND_DESC blendDesc;
        blendDesc.AlphaToCoverageEnable = false;  // �A���t�@�g�D�J�o���b�W��L���ɂ��邩
        blendDesc.IndependentBlendEnable = false; // ���������_�[�^�[�Q�b�g�ɓƗ������u�����h��L���ɂ��邩�ǂ������w�肵�܂�
        for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
          blendDesc.RenderTarget[i] = defaultRenderTarggetBlendDesc;

        // �[�x�X�e���V����Ԃ̐ݒ�
        D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = true;                          // �f�v�X�e�X�g��L���ɂ��邩?
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �[�x�X�e���V�� �o�b�t�@�[�ւ̏������݂��I���ɂ��܂�
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      // �[�x�f�[�^�������̐[�x�f�[�^�Ɣ�r����֐��ł��B�\�[�X�f�[�^���Ώۃf�[�^�����������ꍇ�A��r�ɍ��i���܂��B
        depthStencilDesc.StencilEnable = false;                       // �X�e���V���e�X�g��L���ɂ��邩?

        // �}���`�T���v�����O�p�����[�^�[�̐ݒ�
        DXGI_SAMPLE_DESC sampleDesc = {};
        sampleDesc.Count = 1; // �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�

        // �p�C�v���C���X�e�[�g�̐ݒ�
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) }; // ���̓��C�A�E�g�̍\��
        psoDesc.pRootSignature = rootSignature;                                   // ���[�g�V�O�l�`��
        psoDesc.RasterizerState = rasterizer_desc;                                // ���X�^���C�U�̏��
        psoDesc.VS = vs;                                                          // ���_�V�F�[�_�[�̍\��
        psoDesc.PS = ps;                                                          // �s�N�Z���V�F�[�_�[�̍\��
        psoDesc.BlendState = blendDesc;                                           // �u�����h��Ԃ̍\��
        psoDesc.DepthStencilState = depthStencilDesc;                             // �[�x�X�e���V����Ԃ̍\��
        psoDesc.SampleMask = UINT_MAX;                                            // �u�����h�̏�Ԃ̂��߂̃T���v���̃}�X�N
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // ���̓v���~�e�B�u(�O�p�`)
        psoDesc.NumRenderTargets = 1;                                             // �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g��
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                // �[�x�X�e���V���̃t�H�[�}�b�g
        psoDesc.SampleDesc = sampleDesc;                                          // �T���v�����O��Ԃ̍\��

        // �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
        if (FAILED(device->CreateGraphicsPipelineState(
          &psoDesc,
          IID_PPV_ARGS(&pipelineState))))
        {
          ID3D12DebugDevice* debugInterface;
          if (SUCCEEDED(device->QueryInterface(&debugInterface)))
          {
            debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
            debugInterface->Release();
          }

          throw "�p�C�v���C���X�e�[�g�̐����Ɏ��s���܂����B";
        }
      }
    }

    /**
     *  @brief  �`�悷��
     *  @param  vertexBuffer:���_�o�b�t�@
     *  @param  indexBuffer:���_�C���f�b�N�X�o�b�t�@
     *  @param  indexCount:���_�C���f�b�N�X��
     *  @param  instanceCount:�C���X�^���X��
     */
    void Device::Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer, const unsigned int indexCount, const unsigned int instanceCount)
    {
      // �r���[�|�[�g�̍쐬
      D3D12_VIEWPORT viewport;
      viewport.TopLeftX = 0;
      viewport.TopLeftY = 0;
      viewport.Width = 600;
      viewport.Height = 400;
      viewport.MinDepth = 0;
      viewport.MaxDepth = 1;

      // �V�U�[��`(�V�U�[�e�X�g)�̍쐬
      D3D12_RECT scissor;
      scissor.left = 0;
      scissor.top = 0;
      scissor.right = 600;
      scissor.bottom = 400;

      // �p�C�v���C���X�e�[�g�̐ݒ�(�؂�ւ��Ȃ��ꍇ�́A�R�}���h���X�g���Z�b�g���ɐݒ�\)
      commandList->Get().SetPipelineState(pipelineState);

      // �O���t�B�b�N�X�p�C�v���C���̃��[�g�V�O�l�`����ݒ肷��
      commandList->Get().SetGraphicsRootSignature(rootSignature);

      // �`��Ɏg�p����f�B�X�N���v�^�[�q�[�v��ݒ�
      auto cbvSrvHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Get();
      commandList->Get().SetDescriptorHeaps(1, &cbvSrvHeap);

      // �r���[�|�[�g�̐ݒ�
      commandList->Get().RSSetViewports(1, &viewport);

      // �V�U�[��`(�V�U�[�e�X�g)�̐ݒ�
      commandList->Get().RSSetScissorRects(1, &scissor);

      // �v���~�e�B�u�g�|���W�[�̐ݒ�
      //commandList->Get().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
      commandList->Get().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      // ���_�o�b�t�@�r���[�̐ݒ�
      commandList->Get().IASetVertexBuffers(0, 1, &(vertebBuffer.GetView()));

      // ���_�C���f�b�N�X�r���[�̐ݒ�
      commandList->Get().IASetIndexBuffer(&(indexBuffer.GetView()));

      // �f�B�X�N���v�[�^�q�[�v�e�[�u����ݒ�
      auto handleCbv = cbvSrvHeap->GetGPUDescriptorHandleForHeapStart();  // �萔�o�b�t�@�p�f�B�X�N���v�[�^�q�[�v�e�[�u��
      auto handleSrv = cbvSrvHeap->GetGPUDescriptorHandleForHeapStart();  // StructuredBuffer�p�f�B�X�N���v�[�^�q�[�v�e�[�u��
      auto handleTrv = cbvSrvHeap->GetGPUDescriptorHandleForHeapStart();  // �e�N�X�`���p�f�B�X�N���v�[�^�q�[�v�e�[�u��
      handleSrv.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
      handleTrv.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 2;
      commandList->Get().SetGraphicsRootDescriptorTable(0, handleCbv);
      commandList->Get().SetGraphicsRootDescriptorTable(1, handleSrv);
      commandList->Get().SetGraphicsRootDescriptorTable(2, handleTrv);

      // �`��R�}���h�̐���
      commandList->Get().DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
    }

    /**
     *  @brief  �萔�o�b�t�@���쐬����
     *  @param  size:�萔�o�b�t�@�̃T�C�Y
     *  @return �萔�o�b�t�@�ւ̃|�C���^
     */
    std::unique_ptr<ConstantBuffer> Device::CreateConstantBuffer(const unsigned int size)
    {
      auto constantBuffer = std::make_unique<ConstantBuffer>();
      auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
      auto& descriptor = descriptorHeap.CreateDescriptor();
      constantBuffer->Create(device.get(), descriptor.GetHandleForCPU(), size);

      return constantBuffer;
    }

    /**
     *  @brief  �V�F�[�_�[���\�[�X�o�b�t�@���쐬����
     *  @param  num:���\�[�X���̗v�f��
     *  @param  size:���\�[�X����1�v�f�̃T�C�Y
     *  @return �V�F�[�_�[���\�[�X�o�b�t�@�ւ̃|�C���^
     */
    std::unique_ptr<ShaderResourceBuffer> Device::CreateShaderResourceBuffer(const unsigned int num, const unsigned int size)
    {
      // �q�[�v�̐ݒ�
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // �q�[�v�̃������v�[��(�s���ɐݒ�)
      properties.CreationNodeMask = 1;                              // ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
      properties.VisibleNodeMask = 1;                               // ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

      auto shaderResourceBuffer = std::make_unique<ShaderResourceBuffer>();
      auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
      auto& descriptor = descriptorHeap.CreateDescriptor();
      shaderResourceBuffer->Create(device.get(), descriptor.GetHandleForCPU(), num, size);

      return shaderResourceBuffer;
    }

    /**
     *  @brief  �f�o�C�X���擾����
     *  @return �f�o�C�X�ւ̎Q��
     */
    ID3D12Device& Device::GetDevice() const
    {
      return *device;
    }

    // ��X�ʃN���X�ֈړ�����
#pragma region Texture
    /**
     *  @brief  �e�N�X�`���o�b�t�@�𐶐�����
     *  @param  data:�e�N�X�`���f�[�^
     *  @param  width:����
     *  @param  height:�c��
     *  @param  bytesPerPixel:1�s�N�Z���Ŏg�p����o�C�g��
     */
    void Device::CreateTextureBuffer(const uint8_t* const data, const uint32_t width, const uint32_t height, const uint8_t bytesPerPixel)
    {
      // ���\�[�X�̍쐬
      {
        D3D12_HEAP_PROPERTIES properties;
        properties.Type = D3D12_HEAP_TYPE_DEFAULT;
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        properties.CreationNodeMask = 1;
        properties.VisibleNodeMask = 1;

        Texture* texture = new Texture;
        texture->Create(device.get(), properties, width, height);
        texBuffer.reset(texture);
      }

      // ���ԃ��\�[�X�̍쐬
      {
        std::unique_ptr<Sein::Direct3D12::Buffer> buffer = std::make_unique<Buffer>();

        D3D12_RESOURCE_DESC Desc = texBuffer->Get().GetDesc();
        UINT64 RequiredSize = 0;

        device->GetCopyableFootprints(&Desc, 0, 1, 0, nullptr, nullptr, nullptr, &RequiredSize);

        D3D12_HEAP_PROPERTIES properties;
        properties.Type = D3D12_HEAP_TYPE_UPLOAD;
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        properties.CreationNodeMask = 1;
        properties.VisibleNodeMask = 1;
        buffer->Create(device.get(), properties, RequiredSize);

        // �f�[�^�̃R�s�[
        {
          D3D12_SUBRESOURCE_DATA textureData = {};
          textureData.pData = data;
          textureData.RowPitch = width * bytesPerPixel;
          textureData.SlicePitch = textureData.RowPitch * height;

          unsigned char* pData;

          if (FAILED(buffer->Get().Map(0, nullptr, reinterpret_cast<void**>(&pData))))
          {
            throw "�e�N�X�`���p���ԃ��\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
          }
          std::memcpy(pData, data, sizeof(uint8_t) * textureData.SlicePitch);
          buffer->Get().Unmap(0, nullptr);
        }

        // �e�N�X�`���p���\�[�X�փR�s�[
        {
          commandList->Begin();

          D3D12_RESOURCE_DESC Desc = texBuffer->Get().GetDesc();
          D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint;
          device->GetCopyableFootprints(&Desc, 0, 1, 0, &footPrint, nullptr, nullptr, nullptr);

          D3D12_TEXTURE_COPY_LOCATION Dst;
          Dst.pResource = &(texBuffer->Get());
          Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
          Dst.SubresourceIndex = 0;

          D3D12_TEXTURE_COPY_LOCATION Src;
          Src.pResource = &(buffer->Get());
          Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
          Src.PlacedFootprint = footPrint;
          commandList->Get().CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);

          // ���\�[�X�o���A
          D3D12_RESOURCE_BARRIER barrier;
          barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
          barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
          barrier.Transition.pResource = &(texBuffer->Get());
          barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
          barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
          barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
          commandList->Get().ResourceBarrier(1, &barrier);

          // �R�}���h���X�g���N���[�Y����
          // �R�}���h���X�g�̎��s
          commandList->End();
          ID3D12CommandList* ppCommandLists[] = { &(commandList->Get()) };
          commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

          WaitForGpu();
        }
      }

      // �V�F�[�_�[���\�[�X�r���[�̍쐬
      {
        // Describe and create a SRV for the texture.
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
        auto& descriptor = descriptorHeap.CreateDescriptor();
        device->CreateShaderResourceView(&(texBuffer->Get()), &srvDesc, descriptor.GetHandleForCPU());
      }
    }
#pragma endregion
  };
};