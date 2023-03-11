#include <Arduino.h>
// https://logikara.blog/3_axis_monitor/#mokuji_2
#define M5STACK_MPU6886 // <M5Core2.h>をincludeする前に、IMUモジュールを#defineしておく
#include <M5Core2.h>

#define LGFX_AUTODETECT // 自動認識(D-duino-32 XS, PyBadgeはパネルID読取れないため自動認識の対象から外れているそうです)
#define LGFX_USE_V1     // v1.0.0を有効に(v0からの移行期間の特別措置とのこと。書かない場合は旧v0系で動作)
#include <LovyanGFX.hpp>          // lovyanGFXのヘッダを準備
#include <LGFX_AUTODETECT.hpp>    // クラス"LGFX"を準備
static LGFX lcd;                  // LGFXのインスタンスを作成（クラスLGFXを使ってlcdコマンドでいろいろできるようにする）
static LGFX_Sprite canvas(&lcd);  // スプライトを使う場合はLGFX_Spriteのインスタンスを作成

// グローバル変数宣言
float accX, accY, accZ;     // 加速度格納用
float x, y, z = 0;          // 現在値格納用
float x10, y10, z10 = 0;    // 0補正値格納用
float ax_data1[6] = {0};    // 加速度最大値格納用
float plot_x[250] = {150};  // グラフプロットデータ格納用
float plot_y[250] = {150};
float plot_z[250] = {150};
int axis = 0;               // x軸座標カウント用
int range_select = 1;       // y軸レンジ選択用
float y_range = 250;        // y軸レンジ換算値用
int sampling = 0;           // サンプリング回数カウント用

/**************************** Y軸項目表示 ****************************/
void rangeItem(float y_range) {
  canvas.setTextColor(WHITE, BLACK);  // 文字色
  canvas.setFont(&fonts::Font2);      // フォント設定
  if (y_range == 5) {                 // y_range（換算値）が5なら整数表示
    canvas.setCursor(5, 93);    canvas.printf("%7d", 10);
    canvas.setCursor(5, 193);   canvas.printf("%7d", -10);
  } else {                            // y_range（換算値）が5でなければ浮動小数表示
    canvas.setCursor(5, 93);    canvas.printf("%7.1f", 50 / y_range);
    canvas.setCursor(5, 193);   canvas.printf("%7.1f", -50 / y_range);
  }
}
/*************************** 0補正値セット ***************************/
void zeroReset() {
  M5.IMU.getAccelData(&accX,&accY,&accZ); // 加速度データ取得
  x10 = accX;         //取得値を補正値としてセット
  y10 = accY;
  z10 = accZ;
}
// 初期設定 ----------------------------------------------------------
void setup(){
  M5.begin();               // 本体初期化
  lcd.init();               // LCD初期化
  canvas.setColorDepth(8);  // カラーモード設定
  canvas.createSprite(lcd.width(), lcd.height()); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）

  M5.IMU.Init();            // 6軸センサ初期化
  M5.IMU.SetAccelFsr(M5.IMU.AFS_2G);  // 加速度センサースケール初期値 ±2G(2,4,8,16) ※GRAYは「setAccelFsr」（先頭のsが小文字）

  canvas.fillScreen(BLACK);           // 背景色
  canvas.setTextColor(WHITE , BLACK); // 文字色
  canvas.setTextSize(1);
  canvas.setFont(&fonts::Font4);      // フォント設定
  canvas.setCursor(20, 0);  canvas.print("3-AXIS MONITOR (G)"); // 座標、タイトル表示
  
  canvas.drawRect(60, 90, 250, 120, WHITE); // グラフ枠表示
  canvas.drawRect(59, 90, 252, 120, WHITE);
  canvas.drawRect(58, 90, 254, 120, WHITE);

  canvas.setFont(&fonts::Font2);            //  フォント設定
  canvas.setCursor(40, 144);   canvas.print("0"); // Y軸0表示
  
  // 設定ボタン項目表示
  canvas.drawRect(20, 217, 70, 23, DARKGREY);     // ボタン枠
  canvas.drawRect(127, 217, 70, 23, DARKGREY);
  canvas.drawRect(234, 217, 70, 23, DARKGREY);

  canvas.setTextColor(BLACK, DARKGREY);
  canvas.setCursor(24, 221);    canvas.print("  0 SET  ");  // 0セットボタン
  canvas.setCursor(130, 221);   canvas.print("  RESET  ");  // 最大値0リセットボタン
  canvas.setCursor(237, 221);   canvas.print("  RANGE  ");  // Y軸レンジ選択ボタン

  zeroReset();        // 0リセット
  rangeItem(y_range); // y軸レンジ項目表示呼出し
}
// メイン処理 --------------------------------------------------------
void loop() {
  float ax_data0[6];  // 加速度測定値格納用
  M5.update();        // ボタン状態更新
  M5.IMU.getAccelData(&accX,&accY,&accZ); // 加速度データ取得

  // 0補正
  if (M5.BtnA.wasReleased()) { // ボタンAが押されたら0補正値セット
    delay(1000);  // 本体安定待ち
    zeroReset();  // 0リセット
  }
  x = accX - x10; // 補正後の数値を表示値としてセット
  y = accY - y10;
  z = accZ - z10;

  // 最大値リセット
  if (M5.BtnB.wasReleased()) {  // ボタンBが押されたら0リセット
    delay(1000);                // 本体安定待ち
    for (int i = 0; i < 6; i++) {
      ax_data1[i] = 0;          // 最大値配列0リセット
    }
  }
  // y軸レンジ変更
  if (M5.BtnC.wasReleased()) {  // ボタンCが押されたらy軸レンジ変更
    range_select++;             // y軸レンジ選択+1
    if (range_select == 7) {    // y軸レンジ選択値が7なら
      range_select = 1;         // 1にセット
    }
    // 加速度センサスケール選択 ※GRAYは「setAccelFsr」（先頭のsが小文字）
    switch (range_select) { // y軸レンジ、加速度センサスケール選択
      case 1: y_range = 250;                // ±0.2G
        M5.IMU.SetAccelFsr(M5.IMU.AFS_2G);  // ±2G
        break;
      case 2: y_range = 100;                // ±0.5G
        M5.IMU.SetAccelFsr(M5.IMU.AFS_2G);  // ±2G
        break;
      case 3: y_range = 50;                 // ±1.0G
        M5.IMU.SetAccelFsr(M5.IMU.AFS_2G);  // ±2G
        break;
      case 4: y_range = 25;                 // ±2.0G
        M5.IMU.SetAccelFsr(M5.IMU.AFS_4G);  // ±4G
        break;
      case 5: y_range = 10;                 // ±5.0G
        M5.IMU.SetAccelFsr(M5.IMU.AFS_8G);  // ±8G
        break;
      case 6: y_range = 5;                  // ±10G
        M5.IMU.SetAccelFsr(M5.IMU.AFS_16G); // ±16G
        break;
    }
    rangeItem(y_range); // y軸レンジ項目表示呼出し
  }
  // 最大値の取得
  if (x >= 0) {                       // 0以上なら
    ax_data0[0] = x;                  // ＋加速度を格納
    if (ax_data0[0] > ax_data1[0]) {  // 前回の測定値より大きければ
      ax_data1[0] = ax_data0[0];      // 測定値を更新
    }
  } else {                            // 0より小さければ
    ax_data0[1] = x;                  // -加速度を格納
    if (ax_data0[1] < ax_data1[1]) {  // 前回の測定値より小さければ
      ax_data1[1] = ax_data0[1];      // 測定値を更新
    }
  }
  if (y >= 0) {
    ax_data0[2] = y;
    if (ax_data0[2] > ax_data1[2]) {  // 前回の測定値より大きければ
      ax_data1[2] = ax_data0[2];      // 測定値を更新
    }
  } else {
    ax_data0[3] = y;
    if (ax_data0[3] < ax_data1[3]) {  // 前回の測定値より小さければ
      ax_data1[3] = ax_data0[3];      // 測定値を更新
    }
  }
  if (z >= 0) {
    ax_data0[4] = z;
    if (ax_data0[4] > ax_data1[4]) {  // 前回の測定値より大きければ
      ax_data1[4] = ax_data0[4];      // 測定値を更新
    }
  } else {
    ax_data0[5] = z;
    if (ax_data0[5] < ax_data1[5]) {  // 前回の測定値より小さければ
      ax_data1[5] = ax_data0[5];      // 測定値を更新
    }
  }
  if (sampling == 5) {
    // 加速度表示(現在値　最小〜最大値)
    canvas.setFont(&fonts::Font4);  // フォント設定
    canvas.setTextColor(ORANGE , BLACK);  // X軸測定値
    canvas.setCursor(30, 24);   canvas.printf("X:%7.3f ", x);             // 現在値
    canvas.setCursor(140, 24);  canvas.printf("( %5.2f", ax_data1[1]);    // 最小値
    canvas.setCursor(220, 24);  canvas.printf("~ %5.2f )", ax_data1[0]);  // 最大値
    canvas.setTextColor(CYAN , BLACK);    // Y軸測定値
    canvas.setCursor(30, 44);   canvas.printf("Y:%7.3f ", y);
    canvas.setCursor(140, 44);  canvas.printf("( %5.2f", ax_data1[3]);
    canvas.setCursor(220, 44);  canvas.printf("~ %5.2f )", ax_data1[2]);
    canvas.setTextColor(GREEN , BLACK);   // Z軸測定値
    canvas.setCursor(30, 64);   canvas.printf("Z:%7.3f ", z);
    canvas.setCursor(140, 64);  canvas.printf("( %5.2f", ax_data1[5]);
    canvas.setCursor(220, 64);  canvas.printf("~ %5.2f )", ax_data1[4]);
    
    // バッテリー残量（MAX約4.2V、限界電圧3V）パーセント換算表示 ※GRAYは使用不可180〜188行までコメントアウトする。
    float battery;    // バッテリー残量表示用
    battery = (M5.Axp.GetBatVoltage() - 3) * 90;      // バッテリー残量取得、換算
    if (battery > 100) {                              // 換算値が100以上なら
      battery = 100;                                  // 100にする
    }
    canvas.setCursor(287, 0); canvas.setFont(&fonts::Font2);  // 座標、フォント 2(16px)
    canvas.setTextColor(DARKGREY, BLACK);             // 文字色
    canvas.printf("%3.0f%%", battery);                // バッテリー残量表示

    sampling = 0;  // サンプリング回数0リセット
  }
  sampling++;      // サンプリング回数+1

  // グラフ目盛表示
  canvas.fillRect(61, 91, 248, 118, BLACK);     // 枠内リセット
  canvas.drawFastHLine(55, 100, 260, DARKGREY); // 横線
  canvas.drawFastHLine(55, 150, 260, WHITE);
  canvas.drawFastHLine(55, 200, 260, DARKGREY);
  canvas.drawFastVLine(110, 90, 120, DARKGREY); // 縦線
  canvas.drawFastVLine(160, 90, 120, DARKGREY);
  canvas.drawFastVLine(210, 90, 120, DARKGREY);
  canvas.drawFastVLine(260, 90, 120, DARKGREY);

  // 表示データスクロール
  if (axis < 248) {               // グラフx軸カウントmaxでなければ
    axis++;                       // グラフx軸カウント+1
  } else {                        // グラフx軸カウントmaxなら
    for (int i = 0; i < 249; i++) {   // 表示データ全てを
      plot_x[i] = plot_x[i + 1];  // 前配列へシフト
      plot_y[i] = plot_y[i + 1];  // 前配列へシフト
      plot_z[i] = plot_z[i + 1];  // 前配列へシフト
    }
  }
  // 加速度データ換算
  plot_x[axis] = 150 - (x * y_range); // 換算データを表示データ配列へ
  plot_y[axis] = 150 - (y * y_range);
  plot_z[axis] = 150 - (z * y_range);

  // 折れ線グラフ表示
  for (int i = 1; i <= axis; i++) { // x軸max250まで繰返し
    // X軸
    if ((91 < plot_x[i]) && (plot_x[i] < 208)) {  // 表示データが91より大きくて208以下なら
      canvas.drawLine(i+59, plot_x[i-1], i + 60, plot_x[i], ORANGE);  // 波形描画
    } else if (plot_x[i] <= 91) {
      plot_x[i] = 91;
      canvas.drawLine(i+59, plot_x[i-1], i + 60, plot_x[i], ORANGE);  // 波形描画(max)
    } else if (plot_x[i] >= 208) {
      plot_x[i] = 208;
      canvas.drawLine(i+59, plot_x[i-1], i + 60, plot_x[i], ORANGE);  // 波形描画(min)
    }
    // Y軸
    if ((91 < plot_y[i]) && (plot_y[i] < 208)) {  // 表示データが91より大きくて208以下なら
      canvas.drawLine(i+59, plot_y[i-1], i + 60, plot_y[i], CYAN);    // 波形描画
    } else if (plot_y[i] <= 91) {
      plot_y[i] = 91;
      canvas.drawLine(i+59, plot_y[i-1], i + 60, plot_y[i], CYAN);  // 波形描画(max)
    } else if (plot_y[i] >= 208) {
      plot_y[i] = 208;
      canvas.drawLine(i+59, plot_y[i-1], i + 60, plot_y[i], CYAN);  // 波形描画(min)
    }
    // Z軸
    if ((91 < plot_z[i]) && (plot_z[i] < 208)) {  // 表示データが91より大きくて208以下なら
      canvas.drawLine(i+59, plot_z[i-1], i + 60, plot_z[i], GREEN);   // 波形描画
    } else if (plot_z[i] <= 91) {
      plot_z[i] = 91;
      canvas.drawLine(i+59, plot_z[i-1], i + 60, plot_z[i], GREEN);  // 波形描画(max)
    } else if (plot_z[i] >= 208) {
      plot_z[i] = 208;
      canvas.drawLine(i+59, plot_z[i-1], i + 60, plot_z[i], GREEN);  // 波形描画(min)
    }
  }
  canvas.pushSprite(0, 0);  // メモリ内に描画したcanvasを座標を指定して表示する
}


