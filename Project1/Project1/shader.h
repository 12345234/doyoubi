#pragma once
#include "device.h"
class shader final {
public:
    //---------------------------------------------------------------------------------
    /**
     * @brief    コンストラクタ
     */
    shader() = default;

    //---------------------------------------------------------------------------------
    /**
     * @brief    デストラクタ
     */
    ~shader();

    //---------------------------------------------------------------------------------
    /**
     * @brief	シェーダを作成する
     * @param	device	デバイスクラスのインスタンス
     * @return	成功すれば true
     */
    [[nodiscard]] bool create(const device& device) noexcept;

    //---------------------------------------------------------------------------------
    /**
     * @brief	頂点シェーダを取得する
     * @return	頂点シェーダのデータ
     */
    [[nodiscard]] ID3DBlob* vertexShader() const noexcept;

    //---------------------------------------------------------------------------------
    /**
     * @brief	ピクセルシェーダを取得する
     * @return	ピクセルシェーダのデータ
     */
    [[nodiscard]] ID3DBlob* pixelShader() const noexcept;


private:
    ID3DBlob* vertexShader_{};  /// 頂点シェーダ
    ID3DBlob* pixelShader_{};   /// ピクセルシェーダ
};
