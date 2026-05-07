# Toon Shader 설계 문서 (원신 스타일)

**작성일:** 2026-05-07  
**프로젝트:** ProjectAA  
**대상 엔진:** Unreal Engine 5.5

---

## 개요

원신(Genshin Impact) 스타일의 툰쉐이더를 기존 마스터 머티리얼 위에 Material Function 분리형으로 구현한다.
기존 마스터 머티리얼 구조를 최대한 보존하면서, 재사용 가능한 Material Function 단위로 툰 로직을 분리한다.

---

## 구현 방식 결정

| 항목 | 결정 |
|---|---|
| 구현 방식 | Material Function 분리형 (기존 마스터 머티리얼 보존) |
| 아웃라인 | Inverted Hull (버텍스 노말 확장) |
| Rim Light | Fresnel + 광원 방향 기반 블렌딩 (파라미터로 비율 조정) |
| 얼굴 쉐이딩 | SDF 텍스처 기반 Shadow Mask |

---

## 파일 구조

```
Content/
└── Materials/
    └── Toon/
        ├── Functions/
        │   ├── MF_ToonDiffuse.uasset
        │   ├── MF_ToonRimLight.uasset
        │   └── MF_FaceSDF.uasset
        ├── Outline/
        │   └── M_ToonOutline.uasset
        └── (기존 M_Master_Character.uasset — 수정 최소화)
```

---

## Material Function 상세 설계

### 1. MF_ToonDiffuse

**목적:** NdotL 기반 Step 명암 처리 (셀 쉐이딩)

**입력 파라미터:**
| 이름 | 타입 | 설명 |
|---|---|---|
| BaseColor | Vector3 | 기본 색상 |
| WorldNormal | Vector3 | 월드 노말 |
| ShadowColor | Vector3 | 그림자 색조 (기본값: 어두운 보라빛) |
| ShadowStep | Scalar | 명암 경계 위치 (0~1, 기본값: 0.5) |
| ShadowFeather | Scalar | 경계 부드러움 (0~0.1, 기본값: 0.02) |

**로직:**
```
NdotL = saturate(dot(WorldNormal, MainLightDir))
ShadowMask = smoothstep(ShadowStep - ShadowFeather, ShadowStep + ShadowFeather, NdotL)
Output = lerp(ShadowColor * BaseColor, BaseColor, ShadowMask)
```

**출력:** ToonDiffuseColor (Vector3)

---

### 2. MF_ToonRimLight

**목적:** 카메라 기반 Fresnel + 광원 방향 기반 Rim Light 블렌딩

**입력 파라미터:**
| 이름 | 타입 | 설명 |
|---|---|---|
| WorldNormal | Vector3 | 월드 노말 |
| RimColor | Vector3 | Rim 색상 |
| RimWidth | Scalar | Rim 굵기 (0~1, 기본값: 0.7) |
| RimIntensity | Scalar | Rim 밝기 (기본값: 1.0) |
| RimPower | Scalar | Fresnel 지수 (기본값: 3.0) |
| LightDirWeight | Scalar | 광원 방향 비율 (0=순수 Fresnel, 1=순수 광원, 기본값: 0.5) |

**로직:**
```
CameraDir = normalize(CameraPosition - WorldPosition)
Fresnel = pow(1.0 - saturate(dot(WorldNormal, CameraDir)), RimPower)
LightRim = dot(WorldNormal, -MainLightDir) * 0.5 + 0.5
BlendedRim = lerp(Fresnel, LightRim, LightDirWeight)
RimMask = step(1.0 - RimWidth, BlendedRim)
Output = RimMask * RimColor * RimIntensity
```

**출력:** RimLightColor (Vector3)

---

### 3. MF_FaceSDF

**목적:** 얼굴 노말맵 대신 SDF 텍스처로 광원 수평 각도에 따른 자연스러운 명암 제어

**입력 파라미터:**
| 이름 | 타입 | 설명 |
|---|---|---|
| SDFTexture | Texture2D | SDF 텍스처 (R=왼쪽, G=오른쪽) |
| BaseColor | Vector3 | 기본 색상 |
| ShadowColor | Vector3 | 그림자 색조 |
| ShadowFeather | Scalar | 경계 부드러움 |

**로직:**
```
LightYaw = atan2(MainLightDir.x, MainLightDir.z)  // 수평 각도만 사용
FacingSign = sign(LightYaw)                        // 좌우 판별
SDF_L = SDFTexture.r
SDF_R = SDFTexture.g  (U좌표 반전하여 샘플링)
SelectedSDF = FacingSign > 0 ? SDF_L : SDF_R
ShadowMask = smoothstep(0.5 - ShadowFeather, 0.5 + ShadowFeather, SelectedSDF - abs(LightYaw) / PI)
Output = lerp(ShadowColor * BaseColor, BaseColor, ShadowMask)
```

**출력:** FaceDiffuseColor (Vector3)

---

### 4. M_ToonOutline (Inverted Hull)

**목적:** 캐릭터 외곽선 표현

**설정:**
- Blend Mode: Opaque
- Shading Model: Unlit
- Two Sided: false
- 렌더링 순서: 캐릭터 메쉬보다 먼저 (Translucency Sort Priority -1)

**파라미터:**
| 이름 | 타입 | 설명 |
|---|---|---|
| OutlineColor | Vector3 | 아웃라인 색상 (기본값: 검정) |
| OutlineWidth | Scalar | 아웃라인 굵기 (기본값: 0.1) |

**World Position Offset 로직:**
```
CameraDistance = length(CameraPosition - WorldPosition)
ScaledWidth = OutlineWidth * CameraDistance * 0.01
WPO = VertexNormal * ScaledWidth
```
노말은 Smooth Normal(평균화된 버텍스 노말)을 별도 UV채널에 저장해 사용하는 것을 권장.

---

## 마스터 머티리얼 수정 사항

기존 `M_Master_Character`에 추가할 내용만 기재:

1. `MF_ToonDiffuse` 호출 → 기존 BaseColor 출력 대체
2. `MF_ToonRimLight` 호출 → Emissive에 Add
3. 얼굴 전용 머티리얼 인스턴스에서는 `MF_FaceSDF` 사용 (Switch 파라미터로 On/Off)
4. 아웃라인은 별도 메쉬 슬롯(Material Element 1)에 `M_ToonOutline` 할당

---

## 캐릭터 메쉬 요구사항

| 항목 | 내용 |
|---|---|
| UV1 | 일반 텍스처 UV |
| UV2 | Smooth Normal 저장용 (아웃라인 품질 향상) |
| 얼굴 메쉬 | SDF 텍스처 별도 제작 필요 (Hoyoverse 방식 참고) |

---

## 구현 우선순위

1. MF_ToonDiffuse — 가장 핵심, 먼저 구현
2. M_ToonOutline — 시각적 임팩트 큼
3. MF_ToonRimLight — 완성도 향상
4. MF_FaceSDF — 얼굴 메쉬 + SDF 텍스처 준비 후 마지막
