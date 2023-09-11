# 개요

본 프로젝트는 C++ 기반으로 을 입출력 다중화를 지원하는 간단한 HTTP 1.1 버전 기반의 서버 프로그램을 제작하는 프로젝트입니다.

# 프로젝트 목적과 의의
1. RFC 9110,9111에 정의된 HTTP/1.1 에서의 통신을 직접 구현해봄으로써 서버와 클라이언트가 통신할 때, 어떠한 데이터가 오가는지 이해한다.
2. TCP 기반의 Socket 통신으로 구현해봄으로써 C++에서의 소켓 프로그래밍에 대해 익힌다.
3. 입출력 다중화 기법을 구현해봄으로써 멀티스레드 환경에서의 서버와 어떠한 성능적인 차이가 나는지 이해하고, C++에서의 event관리 라이브러리인 kqueue에 대해 이해한
다.
4. 원활한 협업과 프로젝트 진행을 위해 협업 관리 툴을 사용하는 등의 협업 시스템을 구축해보고, 다양한 방법을 시도해봄으로써 효율적인 협업이 이루어지려면 어떠한 방법을
사용해야하는지 이해한다.

# 팀 구성
팀 구성 전, 프로그램 구현에 필요한 클래스를 다음과 같이 지정 한 후,
* ServerHandler Class
  * 소켓 생성, kevent loop와 같은 서버의 메인 로직이 정의되어있는 클래스
* ConfigInfo Class
  * 정의한 서버 설정 파일을 파싱하고 데이터를 객체형태로 저장하는 클래스
* HTTP Request/Response Class
  * Client로 부터 받은 Request Message를 파싱하여 저장하거나, 보내줄 Response 데이터를 객체 형태로 저장하여 string으로 변환하는 클래스

**Server Main Logic(ServerHandler Class) (1 명)**</br>
**Server Configuration (ConfigInfo Class) (1 명)**</br>
**HTTP Request Class/HTTP Response Class (1 명)**</br>

총 3명으로 팀을 구성

# 협업 방식
* 메인 로직 담당은 서버의 기능을 구상하고, 다른 팀원들로부터 피드백
* github project를 활용하여 task queue를 작성하는 방식으로 협업
  ![image](https://github.com/Victra15/Webserv/assets/68954072/fa6198f7-97c9-468f-b10d-0a748c8bdf7c)
* 각자 파트에서 작업하면서 본인 파트가 아닌 클래스에 필요한 함수가 있을 경우, 함수 프로토타입과 명세를 명시 후 task queue에 작성 후 해당 클래스 담당하는 인원에게 전달
* 테스트 이후 발생한 버그/수정사항의 경우에도 task queue에 작성하고 원인 파악 후, 대응
* 깃 커밋 컨벤션을 정해 매번의 추가/수정사항을 자세히 작성
# 코드 구조
![image](https://github.com/Victra15/Webserv/assets/68954072/1a1f1a85-7e53-434f-9820-df02472a44d4)
![image](https://github.com/Victra15/Webserv/assets/68954072/e46116fc-38b1-4c63-b1f4-721be9edf183)
1. ServerHandler 객체 초기화
2. ServerHandler.ServerReady() 함수 실행을 통해 Configure file Parsing하여 정보를 담을 객체에 입력하고 Client로부터 접속 요청을 받을 ListenSocket 생성, kqueue 생성.
3. ServerHandler.ServerRun() 함수에서 무한루프를 돌며 kevent 배열을 가져오고, kevent배열을 순회하며 발생한 I/O event 처리
  - Listen Socket에 대한 I/O event 이면 Client와 통신할 새로운 Socket 생성
  - Client Socket에 대한 I/O event 이면 현재 Client의 상태에 따라 동작을 수행
   </br></br>**Client의 상태에 따른 동작은 다음 8가지로 정의**
    - RECV_HEADER - 클라이언트로부터 리퀘스트 메세지의 헤더 정보를 받아옴
    - RECV_BODY - 클라이언트로부터 리퀘스트 메세지의 바디 정보를 받아
    - GET - 요청된 URL에 GET메소드를 수행
    - POST - 요청된 URL에 POST메소드를 수행
    - DELETE - 요청된 URL에 DELETE메소드를 수행
    - MAKE_RESPONSE - 일반 파일을 읽어서 response 객체의 body에 기록
    - MAKE_CGI_RESPONSE - cgi스크립트 파일을 실행하여 response 객체의 body에 기록
    - SEND_RESPONSE - 완성된 response 객체를 string으로 변환하여 client에 전송
각각의 동작의 세부 사항은 아래 Flow Chart에 정리되어 있다.

- RECV_HEADER</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/9ad1bb33-6e46-4bb9-b43b-4aa4ad32aae7)

- RECV_BODY</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/f21b730e-1408-4b5e-85bf-9531b771a752)

- GET</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/a3fc2d99-bdb5-4e4a-a744-6c120345d5c1)

- POST</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/f8f28d70-2432-4945-95dc-a642b94159d5)

- DELETE</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/b37c5f36-77aa-47a9-be1b-0aabf90cd2f5)

- MAKE_RESPONSE
- MAKE_CGI_RESPONSE</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/6c0dfb0b-cec9-4c37-85bc-b1888870002d)

- SEND_RESPONSE</br>
![image](https://github.com/Victra15/Webserv/assets/68954072/40424cc6-12bc-4f16-85ee-7dfeb5b963b3)

# 실행 방법

**본 프로젝트는 Mac 환경에서 동작합니다.**

GNU make 사용해 make
```bash
  make
```
configuration file .conf 확장자로 지정하여 conf 디렉토리에 작성 후, 아래와 같은 명령어로 실행
```bash
  ./webserv <Configuration-file-name>.conf
```
# Conf 파일 작성법

## Server block

* host <IPv4-format>
  * 접속 ip 지정
* port <port-number>
  * 접속 port 지정
* server_name <server-name>
  * 서버 이름 지정
* root <root-directory>
  * root URL과 연결될 디렉토리 지정
* index <index-page-source>
  * root URL로 접속시 기본으로 연결될 index페이지 소스 지정
* client_max_body_size <integer-size>
  * 클라이언트로부터 받을 body의 최대 사이즈 지정
* error_page <error-code> <error-page-source-directory>
  * 각 에러코드에 해당하는 페이지 소스 지정
* cgi_pass <cgi-extension> <cgi-program-directory>
  * cgi 프로그램 지정
  
## Location block

* location <directory> {}
  * 특정 디렉토리로 리퀘스트가 들어왔을 경우 동작 지정
* autoindex <on/off>
  * 오토인덱스 기능 켜기/끄기 (기본값: 끄기)
* allow_method <HTTP-Methods>
  * 허용된 메소드 입력
* redirect <redirection-URL>
  * 리다이렉션 페이지 지정

## Configure file 예시

```plain text
server {
    host 127.0.0.1;
    port 4242;
    server_name webserv;
    root /Users/userName/webserv/var;
    index index.html;
    client_max_body_size 1000000;
    error_page 400 ../error_page/400.html;
    error_page 404 ../error_page/404.html;
    error_page 405 ../error_page/405.html;
    cgi_pass .php /Users/userName/webserv/cgi/php-cgi;
    cgi_pass .py /usr/local/bin/python3;

    location /uploads/ {
        autoindex on;
    }

    location /img {
        allow_method GET POST;
    }

    location /redirect {
        redirect https://google.com/;
    }

}
```

# 테스트

본 과제는 브라우저와 서버간 HTTP통신이 정상적으로 작동하는지 확인하기 위해 테스트페이지와 기본 conf 파일을 제공합니다.</br>
기본 conf파일에서는 root에 클론받은 디렉토리를 지정해 주어야 하며</br>
테스트 파일은 /var 디렉토리 안에 구성되어있습니다.</br>
서버 실행 이후 [http://localhost:4242](http://localhost:4242)에 접속하여 브라우저와 서버간 통신을 테스트 해보실 수 있습니다.
