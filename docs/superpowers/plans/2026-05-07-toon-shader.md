# Toon Shader (원신 스타일) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 기존 마스터 머티리얼 위에 Material Function 분리형으로 원신 스타일 툰쉐이더(셀쉐이딩 + Inverted Hull 아웃라인 + SDF 얼굴 쉐이딩 + Rim Light)를 구현한다.

**Architecture:** Material Parameter Collection으로 광원 정보를 공유하고, 각 기능을 독립적인 Material Function으로 분리한다. 기존 마스터 머티리얼은 이 Function들을 호출하는 노드만 추가한다. 아웃라인은 별도 Unlit 머티리얼로 Inverted Hull 방식을 사용한다.

**Tech Stack:** Unreal Engine 5.5, Material Function, Custom HLSL Node, Material Parameter Collection, Blueprint (MPC 업데이트용)

---

## 파일 맵

| 경로 | 액션 | 역할 |
|---|---|---|
| `Content/Materials/Toon/MPC_ToonParams.uasset` | 신규 | 광원 방향/색상 공유용 MPC |
| `Content/Materials/Toon/Functions/MF_ToonDiffuse.uasset` | 신규 | 셀쉐이딩 Diffuse |
| `Content/Materials/Toon/Functions/MF_ToonRimLight.uasset` | 신규 | Rim Light |
| `Content/Materials/Toon/Functions/MF_FaceSDF.uasset` | 신규 | 얼굴 SDF 쉐이딩 |
| `Content/Materials/Toon/Outline/M_ToonOutline.uasset` | 신규 | Inverted Hull 아웃라인 |
| `Content/Materials/Toon/Outline/MI_ToonOutline.uasset` | 신규 | 아웃라인 머티리얼 인스턴스 |
| `Content/BP/System/BP_ToonLightUpdater.uasset` | 신규 | 매 프레임 MPC에 광원 방향 업데이트 |
| 기존 `M_Master_Character` | 수정 | MF_ToonDiffuse + MF_ToonRimLight 연결 |

---

## Task 1: Material Parameter Collection (MPC_ToonParams) 생성

**Files:**
- Create: `Content/Materials/Toon/MPC_ToonParams.uasset`

> UE 에디터 작업. MPC는 모든 툰 머티리얼이 광원 정보를 공유하기 위한 글로벌 파라미터 저장소다.

- [ ] **Step 1: MPC 생성**
  1. Content Browser → `Content/Materials/Toon/` 폴더 생성
  2. 우클릭 → Materials & Textures → Material Parameter Collection
  3. 이름: `MPC_ToonParams`

- [ ] **Step 2: 파라미터 추가**
  MPC_ToonParams 열기 → 아래 파라미터 추가:

  **Vector Parameters:**
  | 이름 | 기본값 |
  |---|---|
  | MainLightDirection | (0.0, -0.707, -0.707, 0.0) |
  | MainLightColor | (1.0, 1.0, 1.0, 1.0) |

  **Scalar Parameters:**
  | 이름 | 기본값 |
  |---|---|
  | MainLightIntensity | 1.0 |

- [ ] **Step 3: 저장 및 확인**
  Ctrl+S 저장. Content Browser에서 `MPC_ToonParams` 아이콘 확인.

---

## Task 2: MF_ToonDiffuse — 셀쉐이딩 Diffuse 함수

**Files:**
- Create: `Content/Materials/Toon/Functions/MF_ToonDiffuse.uasset`

> Step-based 명암 처리. NdotL을 SmoothStep으로 이진화하여 만화 느낌의 명암 경계를 만든다.

- [ ] **Step 1: Material Function 생성**
  1. `Content/Materials/Toon/Functions/` 폴더 생성
  2. 우클릭 → Materials & Textures → Material Function
  3. 이름: `MF_ToonDiffuse`

- [ ] **Step 2: Function Input 노드 추가**
  Material Function 에디터 열기. 다음 **Function Input** 노드들을 추가 (우클릭 → Function Input):

  | Input 이름 | Input Type | Preview Value |
  |---|---|---|
  | BaseColor | FunctionInput_Vector3 | (1, 1, 1) |
  | WorldNormal | FunctionInput_Vector3 | (0, 0, 1) |
  | ShadowColor | FunctionInput_Vector3 | (0.3, 0.25, 0.4) |
  | ShadowStep | FunctionInput_Scalar | 0.5 |
  | ShadowFeather | FunctionInput_Scalar | 0.02 |

- [ ] **Step 3: MPC에서 광원 방향 가져오기**
  1. 우클릭 → `CollectionParameter` 검색 → **Material Parameter Collection** 노드 추가
  2. Collection: `MPC_ToonParams`, Parameter Name: `MainLightDirection`
  3. 노드 이름: `LightDir`

- [ ] **Step 4: NdotL Custom 노드 추가**
  우클릭 → `Custom` 검색 → **Custom** 노드 추가.

  **Inputs 설정 (Custom 노드 Details 패널):**
  | Input 이름 |
  |---|
  | Normal |
  | LightDir |

  **Code:**
  ```hlsl
  float3 N = normalize(Normal);
  float3 L = normalize(LightDir);
  return saturate(dot(N, L));
  ```

  **Output Type:** CMOT Float 1  
  **Description:** NdotL

  연결: `WorldNormal` → Normal, `LightDir 컬렉션 노드` → LightDir

- [ ] **Step 5: SmoothStep 노드로 명암 경계 처리**
  1. 우클릭 → `SmoothStep` 검색 → **Smoothstep** 노드 추가
  2. 연결:
     - Value: `NdotL Custom 노드` 출력
     - Min: `Subtract(ShadowStep, ShadowFeather)` — Add a **Subtract** node
     - Max: `Add(ShadowStep, ShadowFeather)` — Add an **Add** node
  
  **Subtract 노드:** A=ShadowStep, B=ShadowFeather  
  **Add 노드:** A=ShadowStep, B=ShadowFeather

- [ ] **Step 6: Lerp로 밝은 영역/그림자 영역 블렌딩**
  1. **Multiply** 노드 추가: A=ShadowColor, B=BaseColor (그림자 색상 계산)
  2. **Lerp** 노드 추가:
     - A: `Multiply 출력` (그림자)
     - B: `BaseColor` (밝은 영역)
     - Alpha: `Smoothstep 출력`

- [ ] **Step 7: Function Output 연결**
  **Function Output** 노드(기본 생성됨):
  - 이름: `ToonDiffuse`
  - 연결: `Lerp 출력` → Function Output

- [ ] **Step 8: 저장 및 Preview 확인**
  Ctrl+S. Preview 구체에서 명암이 딱 끊기는 모습 확인. ShadowStep을 0.3~0.7 사이로 바꿔보며 경계 이동 확인.

---

## Task 3: MF_ToonRimLight — Rim Light 함수

**Files:**
- Create: `Content/Materials/Toon/Functions/MF_ToonRimLight.uasset`

> 카메라 기반 Fresnel과 광원 방향 기반 Rim을 LightDirWeight로 블렌딩.

- [ ] **Step 1: Material Function 생성**
  `Content/Materials/Toon/Functions/`에 Material Function 생성, 이름: `MF_ToonRimLight`

- [ ] **Step 2: Function Input 노드 추가**

  | Input 이름 | Input Type | Preview Value |
  |---|---|---|
  | WorldNormal | FunctionInput_Vector3 | (0, 0, 1) |
  | RimColor | FunctionInput_Vector3 | (1, 1, 1) |
  | RimWidth | FunctionInput_Scalar | 0.7 |
  | RimIntensity | FunctionInput_Scalar | 1.0 |
  | RimPower | FunctionInput_Scalar | 3.0 |
  | LightDirWeight | FunctionInput_Scalar | 0.5 |

- [ ] **Step 3: Fresnel 계산 Custom 노드**
  **Custom** 노드 추가.

  **Inputs:**
  | Input 이름 |
  |---|
  | Normal |
  | RimPower |

  **Code:**
  ```hlsl
  float3 N = normalize(Normal);
  float3 V = normalize(ResolvedView.WorldCameraOrigin - Parameters.AbsoluteWorldPosition);
  float NdotV = saturate(dot(N, V));
  return pow(1.0 - NdotV, RimPower);
  ```

  **Output Type:** CMOT Float 1  
  **Description:** FresnelRim

  연결: `WorldNormal` → Normal, `RimPower Input` → RimPower

- [ ] **Step 4: 광원 방향 기반 Rim 계산 Custom 노드**
  **Custom** 노드 추가.

  **Inputs:**
  | Input 이름 |
  |---|
  | Normal |
  | LightDir |

  **Code:**
  ```hlsl
  float3 N = normalize(Normal);
  float3 L = normalize(LightDir);
  return saturate(dot(N, -L) * 0.5 + 0.5);
  ```

  **Output Type:** CMOT Float 1  
  **Description:** LightDirRim

  연결: `WorldNormal` → Normal, `MPC_ToonParams/MainLightDirection` → LightDir

- [ ] **Step 5: 두 Rim 블렌딩**
  **Lerp** 노드:
  - A: `FresnelRim 출력`
  - B: `LightDirRim 출력`
  - Alpha: `LightDirWeight Input`

- [ ] **Step 6: Step 노드로 Rim 마스크**
  1. **OneMinus** 노드: 입력 = `RimWidth Input`
  2. **Step** (또는 **If**) Custom 노드 추가:

  **Code:**
  ```hlsl
  return BlendedRim >= Threshold ? 1.0 : 0.0;
  ```

  **Inputs:** BlendedRim, Threshold  
  연결: `Lerp 출력` → BlendedRim, `OneMinus 출력` → Threshold

- [ ] **Step 7: RimColor와 Intensity 적용 및 Output**
  1. **Multiply** 노드: A=`Step 출력`, B=`RimColor Input`
  2. **Multiply** 노드: A=`위 결과`, B=`RimIntensity Input`
  3. **Function Output** 이름 `RimLight`에 연결

- [ ] **Step 8: 저장 및 Preview 확인**
  Ctrl+S. Preview 구체 테두리에 흰 빛 확인. LightDirWeight 0→1 변경 시 Rim 위치 변화 확인.

---

## Task 4: M_ToonOutline — Inverted Hull 아웃라인

**Files:**
- Create: `Content/Materials/Toon/Outline/M_ToonOutline.uasset`
- Create: `Content/Materials/Toon/Outline/MI_ToonOutline.uasset`

> 버텍스 노말을 카메라 거리에 비례해 외부로 밀어 아웃라인 메쉬를 만든다. Unlit + Front Face Culling.

- [ ] **Step 1: Material 생성 및 기본 설정**
  1. `Content/Materials/Toon/Outline/` 폴더 생성
  2. Material 생성, 이름: `M_ToonOutline`
  3. Material 열기 → Details 패널 설정:
     - **Blend Mode:** Opaque
     - **Shading Model:** Unlit
     - **Two Sided:** false (기본값)

- [ ] **Step 2: Material 파라미터 추가**
  1. **VectorParameter** 추가: 이름 `OutlineColor`, 기본값 (0.02, 0.02, 0.02, 1.0)
  2. **ScalarParameter** 추가: 이름 `OutlineWidth`, 기본값 0.1

- [ ] **Step 3: World Position Offset Custom 노드**
  **Custom** 노드 추가.

  **Inputs:**
  | Input 이름 |
  |---|
  | VertexNormal |
  | OutlineWidth |

  **Code:**
  ```hlsl
  float3 N = normalize(VertexNormal);
  float CamDist = length(ResolvedView.WorldCameraOrigin - Parameters.AbsoluteWorldPosition);
  float ScaledWidth = OutlineWidth * CamDist * 0.005;
  ScaledWidth = clamp(ScaledWidth, 0.02, 0.8);
  return N * ScaledWidth;
  ```

  **Output Type:** CMOT Float 3  
  **Description:** OutlineWPO

  연결:
  - `Vertex Normal WS` 노드 → VertexNormal (우클릭 → Vertex Normal WS 검색)
  - `OutlineWidth Parameter` → OutlineWidth

- [ ] **Step 4: 노드 연결**
  1. `OutlineWPO 출력` → **World Position Offset** 핀
  2. `OutlineColor Parameter` → **Emissive Color** 핀

- [ ] **Step 5: 저장**
  Ctrl+S.

- [ ] **Step 6: Material Instance 생성**
  `M_ToonOutline` 우클릭 → Create Material Instance → 이름: `MI_ToonOutline`

- [ ] **Step 7: 테스트 — 캐릭터 메쉬에 적용**
  1. 레벨에 캐릭터 또는 스태틱 메쉬 배치
  2. 해당 메쉬의 **Material Element 1** (두 번째 슬롯)에 `MI_ToonOutline` 할당
     - 메쉬에 Material 슬롯이 1개뿐이라면: Details → LOD0 → Material Slots → + 버튼으로 추가
  3. Viewport에서 외곽에 검은 선 확인. OutlineWidth 값 조정으로 굵기 변화 확인.

---

## Task 5: MF_FaceSDF — 얼굴 SDF 쉐이딩 함수

**Files:**
- Create: `Content/Materials/Toon/Functions/MF_FaceSDF.uasset`

> 노말맵 대신 SDF 텍스처(R=왼쪽조명, G=오른쪽조명)로 얼굴 명암 처리. 광원 수평 각도에 따라 자연스럽게 그림자 경계 이동.

- [ ] **Step 1: Material Function 생성**
  이름: `MF_FaceSDF`

- [ ] **Step 2: Function Input 노드 추가**

  | Input 이름 | Input Type | Preview Value |
  |---|---|---|
  | SDFTexture | FunctionInput_Texture2D | (흰색 텍스처) |
  | UV | FunctionInput_Vector2 | (0.5, 0.5) |
  | BaseColor | FunctionInput_Vector3 | (1, 1, 1) |
  | ShadowColor | FunctionInput_Vector3 | (0.3, 0.25, 0.4) |
  | ShadowFeather | FunctionInput_Scalar | 0.05 |

- [ ] **Step 3: 광원 수평 각도 및 SDF 샘플링 Custom 노드**
  **Custom** 노드 추가.

  **Inputs:**
  | Input 이름 |
  |---|
  | LightDir |
  | SDF_L |
  | SDF_R |
  | ShadowFeather |

  **Code:**
  ```hlsl
  // 수평 각도만 사용 (Y축 무시)
  float2 LightXZ = normalize(float2(LightDir.x, LightDir.z));
  float LightYaw = atan2(LightXZ.x, LightXZ.y);  // -PI ~ PI
  float LightFactor = LightYaw / 3.14159265;       // -1 ~ 1로 정규화

  // 광원이 오른쪽이면 SDF_L, 왼쪽이면 SDF_R 사용
  float SDF = LightFactor >= 0.0 ? SDF_L : SDF_R;
  float Threshold = 1.0 - abs(LightFactor);

  float ShadowMask = smoothstep(
      Threshold - ShadowFeather,
      Threshold + ShadowFeather,
      SDF
  );
  return ShadowMask;
  ```

  **Output Type:** CMOT Float 1  
  **Description:** FaceShadowMask

  연결:
  - `MPC_ToonParams/MainLightDirection` → LightDir
  - 아래 Step에서 SDF_L, SDF_R 연결 예정

- [ ] **Step 4: SDF 텍스처 샘플링**
  1. **TextureSampleParameter2D** 노드: 이름 `SDFTexture` (Function Input과 연결)
     - 실제로는 Function Input의 `SDFTexture`를 `TextureSample` 노드의 Tex 핀에 연결
  2. **TextureSample** 노드 추가 (SDFTexture Input 연결):
     - UV: `UV Input`
     - R채널 출력 → Custom 노드의 `SDF_L`
  3. **TextureSample** 복제:
     - UV: **U 반전** (OneMinus를 UV의 U 컴포넌트에만 적용)
       - **BreakOutFloat2Components** → U에 **OneMinus** → **MakeFloat2** → UV 핀
     - G채널 출력 → Custom 노드의 `SDF_R`

- [ ] **Step 5: Lerp로 최종 색상 및 Output**
  1. **Multiply** 노드: A=`ShadowColor`, B=`BaseColor`
  2. **Lerp** 노드: A=`Multiply 출력`, B=`BaseColor Input`, Alpha=`FaceShadowMask 출력`
  3. **Function Output** 이름 `FaceDiffuse`에 연결

- [ ] **Step 6: 저장**
  Ctrl+S.

---

## Task 6: 기존 마스터 머티리얼에 툰 로직 연결

**Files:**
- Modify: `Content/Materials/(기존 M_Master_Character)`

> 기존 PBR 출력을 MF_ToonDiffuse와 MF_ToonRimLight로 교체. 기존 BaseColor, Normal 입력은 그대로 재사용.

- [ ] **Step 1: 마스터 머티리얼 열기**
  기존 `M_Master_Character` 에디터로 열기.

- [ ] **Step 2: 기존 BaseColor 출력 추적**
  현재 **Base Color** 핀에 연결된 노드를 확인하고, 해당 출력 노드를 기억해둔다 (이후 MF_ToonDiffuse의 BaseColor 입력으로 사용).

- [ ] **Step 3: MF_ToonDiffuse 노드 추가**
  1. 우클릭 → `MF_ToonDiffuse` 검색 → 추가
  2. 연결:
     - `BaseColor`: 기존 Base Color 핀에 연결됐던 노드 출력
     - `WorldNormal`: 기존 Normal 핀에 연결됐던 노드 출력 (없으면 **Vertex Normal WS**)
     - `ShadowColor`: **VectorParameter** 추가 (`ToonShadowColor`, 기본값 (0.3, 0.25, 0.4, 1))
     - `ShadowStep`: **ScalarParameter** 추가 (`ToonShadowStep`, 기본값 0.5)
     - `ShadowFeather`: **ScalarParameter** 추가 (`ToonShadowFeather`, 기본값 0.02)
  3. `MF_ToonDiffuse` 출력 → **Base Color** 핀

- [ ] **Step 4: MF_ToonRimLight 노드 추가**
  1. 우클릭 → `MF_ToonRimLight` 검색 → 추가
  2. 연결:
     - `WorldNormal`: MF_ToonDiffuse의 WorldNormal과 동일 노드 연결
     - `RimColor`: **VectorParameter** (`ToonRimColor`, 기본값 (1, 1, 1, 1))
     - `RimWidth`: **ScalarParameter** (`ToonRimWidth`, 기본값 0.7)
     - `RimIntensity`: **ScalarParameter** (`ToonRimIntensity`, 기본값 1.0)
     - `RimPower`: **ScalarParameter** (`ToonRimPower`, 기본값 3.0)
     - `LightDirWeight`: **ScalarParameter** (`ToonLightDirWeight`, 기본값 0.5)
  3. 기존 **Emissive Color** 핀에 연결된 노드가 있으면: **Add** 노드로 기존 출력 + `MF_ToonRimLight 출력` → Emissive Color
  4. 없으면: `MF_ToonRimLight 출력` 직접 → Emissive Color

- [ ] **Step 5: 저장 및 확인**
  Ctrl+S. 마스터 머티리얼 Preview에서 구체에 셀쉐이딩 명암 + Rim 빛 확인.

- [ ] **Step 6: 머티리얼 인스턴스에서 파라미터 테스트**
  기존 캐릭터 머티리얼 인스턴스 열기 → Toon 파라미터 그룹 확인 → ShadowStep 값 변경으로 명암 이동 확인.

---

## Task 7: Blueprint — MPC 광원 방향 업데이트

**Files:**
- Create: `Content/BP/System/BP_ToonLightUpdater.uasset`

> 레벨에 배치된 Directional Light의 방향을 매 프레임 MPC_ToonParams에 업데이트. 광원 방향이 바뀌어도 툰 쉐이딩이 올바르게 반응하도록 함.

- [ ] **Step 1: Actor Blueprint 생성**
  `Content/BP/System/`에 Actor Blueprint 생성, 이름: `BP_ToonLightUpdater`

- [ ] **Step 2: Event Tick 로직 작성**
  BP 에디터 열기 → Event Graph:

  ```
  Event Tick
    → Get All Actors Of Class (DirectionalLight)
    → Get (Index 0)
    → Get Actor Forward Vector
    → Negate (곱하기 -1, 실제 광원 방향은 Forward의 반대)
    → Set Vector Parameter Value
        Collection: MPC_ToonParams
        Parameter Name: MainLightDirection
        Parameter Value: (위 결과)
  ```

  노드 연결 상세:
  1. **Event Tick** → **Get All Actors Of Class** (Actor Class: Directional Light)
  2. **Get All Actors Of Class** 의 Out Actors → **Get** (Index: 0)
  3. **Get** 출력 → **Get Actor Forward Vector**
  4. **Get Actor Forward Vector** → **Negate** (Vector * -1: **VectorMultiply** 노드, B=(−1,−1,−1))
  5. **Set Vector Parameter Value** 노드:
     - Collection: `MPC_ToonParams`
     - Parameter Name: `MainLightDirection`
     - Parameter Value: Negate 출력

- [ ] **Step 3: 레벨에 배치 및 테스트**
  1. `BP_ToonLightUpdater` 인스턴스를 레벨에 배치
  2. PIE(Play In Editor) 실행
  3. 레벨의 Directional Light를 회전시키면서 캐릭터 명암 경계가 따라 움직이는지 확인

---

## Task 8: 얼굴 머티리얼 인스턴스에 MF_FaceSDF 적용

**Files:**
- Modify: 얼굴 전용 머티리얼 인스턴스 (또는 신규 생성)

> 얼굴 메쉬는 노말맵 대신 SDF로 명암 처리. SDF 텍스처가 없으면 이 Task는 스킵 가능.

- [ ] **Step 1: SDF 텍스처 준비 확인**
  얼굴용 SDF 텍스처가 있는지 확인:
  - R채널: 왼쪽에서 조명이 올 때의 Shadow 경계 SDF
  - G채널: 오른쪽에서 조명이 올 때의 SDF (대칭 이미지)
  - 없으면 이 Task 스킵 후 나중에 진행

- [ ] **Step 2: 마스터 머티리얼에 Face SDF 스위치 추가**
  `M_Master_Character` 열기:
  1. **StaticBoolParameter** 추가: 이름 `UseFaceSDF`, 기본값 false
  2. **MF_FaceSDF** 노드 추가, 입력 연결:
     - `SDFTexture`: **TextureSampleParameter2D** (`FaceSDFTexture`)
     - `UV`: **Texture Coordinate** (Index 0)
     - `BaseColor`, `ShadowColor`, `ShadowFeather`: Task 6과 동일 파라미터
  3. **StaticSwitch** 노드:
     - True: `MF_FaceSDF 출력`
     - False: `MF_ToonDiffuse 출력`
     - Value: `UseFaceSDF`
  4. StaticSwitch 출력 → **Base Color** 핀

- [ ] **Step 3: 얼굴 머티리얼 인스턴스 설정**
  얼굴 전용 MI 열기:
  - `UseFaceSDF`: 체크
  - `FaceSDFTexture`: 준비한 SDF 텍스처 할당
  - Ctrl+S

- [ ] **Step 4: 테스트**
  PIE에서 캐릭터 얼굴에 조명 방향 변경 시 자연스럽게 그림자 경계 이동 확인.

---

## 구현 완료 체크리스트

- [ ] MPC_ToonParams 생성 및 파라미터 확인
- [ ] MF_ToonDiffuse — Preview에서 셀쉐이딩 명암 확인
- [ ] MF_ToonRimLight — Preview에서 테두리 빛 확인
- [ ] M_ToonOutline — 캐릭터 메쉬에 아웃라인 출력 확인
- [ ] MF_FaceSDF — 얼굴 메쉬에 SDF 명암 전환 확인
- [ ] 마스터 머티리얼 연결 — 기존 파라미터 손상 없이 툰 파라미터 추가 확인
- [ ] BP_ToonLightUpdater — Directional Light 회전 시 명암 반응 확인

---

## 트러블슈팅

| 증상 | 원인 | 해결 |
|---|---|---|
| 아웃라인이 내부에서 보임 | Cull Mode 설정 누락 | M_ToonOutline Details → Two Sided 체크 해제 확인 |
| 명암이 없거나 전체 그림자 | MPC 광원 방향이 (0,0,0) | BP_ToonLightUpdater 레벨 배치 확인 |
| Rim이 전체에 퍼짐 | RimWidth가 너무 낮음 | MI에서 ToonRimWidth 값 0.6~0.8로 조정 |
| 얼굴 SDF 경계가 끊김 | ShadowFeather가 너무 낮음 | FaceSDF MI에서 ShadowFeather 0.05~0.1로 조정 |
| 아웃라인 원거리에서 너무 굵음 | ScaledWidth 클램프 부족 | M_ToonOutline Custom 노드의 clamp 최댓값 0.5로 낮춤 |
