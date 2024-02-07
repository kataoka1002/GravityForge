# Gravity Forge
![スクリーンショット 2024-01-29 114759](https://github.com/kataoka1002/333Game/assets/122655580/d95f6abb-e467-46c8-8ca7-ea947a5aa36e)

<br />

## 目次
> * [1.自己紹介](#anchor1)
> * [2.作品概要](#anchor2)
> * [3.ゲーム内容](#anchor3)
>   * [ゲーム内容](#gameNaiyou)
>   * [ルール](#ru-ru)
>   * [操作説明](#sousaSetumei)
> * [4.こだわり](#anchor4)
> * [5.技術紹介](#anchor5)
 
<br />

<a id="anchor1"></a>
## 1.自己紹介
> ### 所属
> #### ・河原電子ビジネス専門学校　ゲームクリエイター科
> ### 名前
> #### ・片岡　海人　(かたおか　うみと)
 
<br />

<a id="anchor2"></a>
## 2.作品概要
> <dl>
>  <dt>タイトル</dt>
>  <dd>Gravity Forge</dd>
>  <dt>制作人数</dt>
>  <dd>1人</dd>
>  <dt>制作期間</dt>
>  <dd>2023年9月～2024年1月
>  <dt>ゲームジャンル</dt>
>  <dd>アクション</dd>
>  <dt>プレイ人数</dt>
>  <dd>1人</dd>
>  <dt>対応ハード</dt>
>  <dd>PC Windows10,11</dd>
>  <dt>対応コントローラー</dt>
>  <dd>Xbox 360 コントローラー</dd>
>  <dt>使用言語</dt>
>  <dd>C++
>  <br />
>  HLSL<dd>
>  <dt>エンジン</dt>
>  <dd>学校内製のエンジンを自分用にカスタムしたもの</dd>
>  <dt>プログラム</dt>
>  <dd>Visual Studio 2022</dd>
>  <dt>3Dモデル</dt>
>  <dd>3ds MAX</dd>
>  <dt>エフェクト</dt>
>  <dd>Effeckseer</dd>
>  <dt>画像</dt>
>  <dd>Adobe Photoshop</dd>
>  <dt>バージョン管理</dt>
>  <dd>Git Hub
>  <br />
>  Fork</dd>
>  <dt>GitHubのURL</dt>
>  <dd>https://github.com/kataoka1002/333Game.git</dd> 
> </dl>

<br />

<a id="anchor3"></a>
## 3.ゲーム内容
> <a id="gameNaiyou"></a>
> ### ①ゲーム内容
> #### 超能力を持ったプレイヤーが街中のオブジェクトを利用してゾンビと戦うアクションゲーム。
> ***
> <a id="ru-ru"></a>
> ### ②ルール
> #### ・街中のオブジェクトを持ち上げることが出来る。
> ![オブジェクト持ち上げ](https://github.com/kataoka1002/333Game/assets/122655580/b0080b0a-173b-4fed-bf4a-b6f9f0d8878e)
> ***
> #### ・オブジェクトを投げつけて攻撃できる(ダメージ高)。
> ![スクリーンショット 2024-01-29 140831](https://github.com/kataoka1002/333Game/assets/122655580/e7ed5e65-20bf-4ca2-85b7-9fa900d0ac96)
> ***
> #### ・パンチで近接攻撃もできる(ダメージ低)。
> ![スクリーンショット 2024-01-30 094410](https://github.com/kataoka1002/333Game/assets/122655580/ac26bf3b-1bdf-4c0d-b06e-59fc4d1d8446)
> ***
> #### ・敵は近づくと攻撃してくる。
> ![スクリーンショット 2024-01-30 094810](https://github.com/kataoka1002/333Game/assets/122655580/1a9b1011-ee05-4a9b-80bb-14bd29047035)
> ***
> #### ・敵は体力が半分以下になると這いつくばって追跡してくる。
> ![スクリーンショット 2024-01-30 095257](https://github.com/kataoka1002/333Game/assets/122655580/754b9469-e6b7-470c-a691-c6500649ed15)
> ***
> #### ・オブジェクト保持中は攻撃されると解除され、オブジェクトは破壊される。
> ![オブジェクト落下](https://github.com/kataoka1002/333Game/assets/122655580/41feb5a5-31b7-424f-aecc-79b3ed246d2e)
> ***
> #### ・奥まで進むとボスが出現し、討伐することでゲームクリアとなる。
> ![スクリーンショット 2024-01-30 111445](https://github.com/kataoka1002/333Game/assets/122655580/b0c577ad-8bbe-41b0-a3f5-164b8a797b86)
> ***
> #### ・ゾンビ討伐数が多く、ボス討伐時間が短いほどスコアがUPする。
> ![スクリーンショット 2024-01-30 120143](https://github.com/kataoka1002/333Game/assets/122655580/99859b12-40c0-4747-a48b-8ce22cf48fa1)
> ***
> <a id="sousaSetumei"></a>
> ### ③操作説明
> ![操作説明](https://github.com/kataoka1002/333Game/assets/122655580/3e10138d-7fc6-41f1-bdb3-da15bd78e7a8)

<br />

<a id="anchor4"></a>
## 4.こだわり
> ### ①オブジェクトの持ち上げ
> #### ・持ち上げる瞬間は遅くし、引き寄せるときは速くすることで重量感を表現した。
> ![持ち上げ](https://github.com/kataoka1002/333Game/assets/122655580/7b0adc0e-c8b3-472f-b74b-acf169d5cceb)
> ***
> ### ②オブジェクトの浮遊
> #### ・滑らかに移動させフワフワすることで、無重力を表現した。
> ※工夫前 <br />
> ![ふゆ(前) (1)](https://github.com/kataoka1002/333Game/assets/122655580/1a1947d9-e5bc-4514-a231-2ca93521ee8c)
> <br />
> ※工夫後 <br />
> ![浮遊(後)](https://github.com/kataoka1002/333Game/assets/122655580/cda556bb-5584-48d9-bc57-756688f1e6bc)
> ***
> ### ③カメラの視点注視点
> #### ・オブジェクトを投げる際に、敵を狙いやすくなるよう視点注視点を設定した。
> ※保持前<br />
> ![スクリーンショット 2024-02-01 165512](https://github.com/kataoka1002/333Game/assets/122655580/26986880-0773-420b-8afd-c92c5c1f6ec3)
> ※保持中<br />
> ![スクリーンショット 2024-02-01 165222](https://github.com/kataoka1002/333Game/assets/122655580/c7303383-f887-43c5-bbbb-505a6b8ba1a1)
> ***
> ### ④体力ゲージ
> #### ・変化量が分かりやすいよう別色が徐々に減少することで表した。
> ![Gravity Forge 2024-02-02 10-41-11](https://github.com/kataoka1002/333Game/assets/122655580/5f23f17e-db34-4599-b0aa-d993cc91a15a)
> ***
> ### ⑤リザルトの演出
> #### ・結果が表示されるだけではなく、点数が増加していき最後に拡大されることで迫力をだした。
> ![リザルト](https://github.com/kataoka1002/333Game/assets/122655580/2d73a279-88b7-4a28-a311-84a0b5ff8b1d)
> ***
> ### ⑥オブジェクト対象のUI
> #### ・持ち上げられるかを分かりやすくするために、対象物には動くUIが表示されるようにした。
> ※持ち上げ不可<br />
> ![オブジェクトUI無](https://github.com/kataoka1002/333Game/assets/122655580/06bbf040-db2c-480f-a63e-0d77ad0305ce)
> ※持ち上げ可
> ![オブジェクトUI有](https://github.com/kataoka1002/333Game/assets/122655580/2a5e9921-5790-42d3-9309-7a3347e79c68)
> ***
> ### ⑦レティクル
> #### ・通常時は中央が分かりつつ邪魔にならない大きさで、保持中は敵を狙いやすいよう大きめ調整した。
> ![レティクル](https://github.com/kataoka1002/333Game/assets/122655580/11073cf0-0e3a-49d7-9348-54262da277aa)
> ***
> ### ⑧ボスの登場シーン
> #### ・ボスステージに移動すると、登場シーンが流れる。<br />・名前の表示と同時に動きを止めることで力強さを出した。
> ![ボス登場](https://github.com/kataoka1002/333Game/assets/122655580/2b5620cf-36bc-468c-bf99-3558894764e4)
> ***
> ### ⑨ボスの攻撃
> #### ・ボスは複数種の攻撃を仕掛けてくるため、避けながら攻撃しなければならない。
>![ボス攻撃](https://github.com/kataoka1002/333Game/assets/122655580/44aca60f-eb62-4f0e-94d0-7794e3608b64)

<br />

<a id="anchor5"></a>
## 5.技術紹介
> ### ①ディファードレンダリング
> #### ・オブジェクトを大量に置くことから、処理時間の増加が考えられたので実装しました。<br /> ・まず、G-Bufferと呼ばれる複数枚のテクスチャに下の４枚のテクスチャを書き込みます。 
> ![G-Buffer](https://github.com/kataoka1002/333Game/assets/122655580/866d09b0-ae83-4aff-809e-a07a9b00923a)
> #### ・そして、G-Bufferの内容を元にライティングの計算を行います。
> ![ディファード比較](https://github.com/kataoka1002/333Game/assets/122655580/fa92d6ec-0ba1-4e07-ad05-60fd662ddb7c)
> ### ②PBR
> ### ③VSM
> ### ④カスケードシャドウ
> ### ⑤インスタンシング描画
> ### ⑥コンピュートシェーダー
> ### ⑦カリング
> ### ⑧ステートパターン
