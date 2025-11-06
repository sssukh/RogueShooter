# [ 🚀 Rogue Shooter ]

[ 3인칭 로그라이크 슈팅 디펜스 데모 프로젝트입니다. ]<br>
[ 현재 뱀파이버 서바이버 형식이지만 계속 수정 및 기능을 추가할 예정입니다. ]

<br>

## 🎮 주요 시연 GIF

![vamPlay](https://github.com/user-attachments/assets/1590d8f5-86b3-4dbe-bdf3-952858cca417)
<br>[ 현재까지 구현한 예시이고 수정중에 있습니다.]


<br>

## 📖 프로젝트 개요

- **개발 기간 :** 2025. 07. ~ (진행중)
- **개발 인원 :** [ 1인 (개인 프로젝트) ]
- **GitHub :** [ https://github.com/sssukh/RogueShooter/new/master ]

<br>

## 🛠️ 사용 기술

- **Game Engine :**
    
    ![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.5-black?style=flat-square&logo=unrealengine)
    
- **Language :**
    
    ![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus)
    
- **Tools :** 
    
    ![Rider](https://img.shields.io/badge/Rider-purple?logo=Rider), ![Git](https://img.shields.io/badge/Git-Orange?logo=Git)
    
- **Version Control :** GitHub

<br>

## ✨ 주요 기능 및 구현 내용

### 1. [핵심 기능 1: 레벨업을 통한 능력 획득 및 진화 시스템]

![vamPlayLU](https://github.com/user-attachments/assets/c163d104-4b84-47a5-ac5c-075036124b8d)
<br>[레벨업을 해 능력을 획득]

![vamPlayEV](https://github.com/user-attachments/assets/52739822-9411-4977-b3be-4708558e2a29)
<br>[일정 조건을 충족하면 능력 진화]

----

### 2. [핵심 기능 2: 비헤비어 트리와 Eqs를 이용한 몬스터 스폰 및 움직임]

![vamMonSpawn](https://github.com/user-attachments/assets/546f0317-623c-42d3-bbeb-1812ec2c821d)

- 비헤이비어 트리(Behavior Tree)를 이용한 몬스터의 추적 로직 구현
- EQS를 이용해 맵 내에서 몬스터 랜덤 스폰
---

### 3. [핵심 기능 3: 멀티플레이어 동기화]
  ![vamPlayMulti](https://github.com/user-attachments/assets/e5e6500f-7e17-4128-86ef-8ac4341a6aa0)
- RPC(Remote Procedure Call)를 이용한 캐릭터의 위치 및 스킬 사용 동기화
---

### 4. [핵심 기능 4: UI애셋 캐싱]

- UI에서 사용하는 애셋들(ex. 이미지 등)을 맵을 이용한 캐시에서 가져오거나 없으면 동적으로 비동기 로드합니다.
- 로드 시 캐시에 저장하여 자주 사용하지 않는 애셋은 큐에 저장된 우선순위에 따라 캐시에서 제거합니다.
- [깃허브 내 관련 코드 링크](https://github.com/sssukh/RogueShooter/blob/master/Source/RogueShooter/Private/System/Subsystem/UIAssetCacheSubsystem.cpp)

