# MyShell

A simple Linux shell implementation
<br/>간단한 리눅스 쉘 구현

All files are single source code files. Compilation is required for each source code.
<br/>모든 파일은 단일 소스 코드 파일입니다. 각 소스 코드마다 컴파일이 필요합니다.

Basically, it is configured to run other executable files in the form of handing over parameters in "mysh".
<br/>기본적으로 "mysh"에서 파라미터를 넘기는 형태로 다른 실행 파일을 실행할 수 있도록 구성되어 있습니다.

There are many Korean comments, so please understand.
<br/>파일 내 한국어로된 주석이 많으니, 양해 부탁드립니다.

# mysh

메인 쉘로 다른 명령어를 실행할 수 있습니다.

| 명령어 | 설명 | 함수 |
| --- | --- | --- |
| cd | 작업 디렉토리 변경 | cmd_cd |
| pwd | 현재 작업 디렉토리 출력 | cmd_pwd |
| echo | 텍스트를 출력 | cmd_echo |
| help | 도움말 출력 | cmd_echo |
| exit | 셀을 종료 | cmd_exit |

# myls

현재 디렉토리의 파일 정보 테이블을 출력합니다.

| 옵션 | 설명 |
| --- | --- |
| a | 숨겨진 파일이나 디렉토리를 출력 |
| i | 파일의 인덱스 값을 출력 |
| s | 블록에 할당된 크기를 출력 |
| F | 각 파일에 지시자를 붙여서 출력 |
| l | 상세 내용을 출력 |
| R | 하위 디렉토리까지 출력 |
| r | 반대로 출력 |

# mycp

파일을 해당 디렉토리 또는 파일로 복사합니다. (상세 옵션 없음)

# mymv

파일을 해당 디렉토리 또는 파일로 이동합니다. (상세 옵션 없음)

# mysleep

10초간 sleep을 수행하는 명령어 (실험적 기능)