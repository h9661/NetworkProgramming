/*
* <<1. 소켓의 옵션과 입출력 버퍼의 크기>>
* 소켓이 지니는 다양한 특성을 파악하고, 또 그 특성을 필요에 맞게 변경하는 것은 데이터 송수신 만큼 중요하다.
* 
* 1. 소켓의 다양한 옵션
* 소켓의 옵션은 계층별로 분류된다. 그러나 지금 이것을 외우거나 이해할 필요는 없다. 중요한 옵션 중 몇 가지에 대해서만 그 의미와 변경 방법을 소개하겠다.
* 
* 2. getsockopt & setsockopt
* 모든 옵션은 설정 상태의 참조(get) 및 변경(set)이 가능하다. 그리고 옵션 참조 및 변경에는 다음 함수를 사용한다.
* 
* int getsockopt(int sock, int level, int optname, void* optval, socklen_t* optlen)
* sock		: 옵션 확인을 위한 소켓 핸들 전달
* level		: 확인할 옵션의 프로토콜 레벨 전달
* optname	: 확인할 옵션의 이름 전달
* optval	: 확인 결과 저장을 위한 버퍼의 주소 전달
* optlen	: 네 번째 매개변수 optval로 전달된 주소 값의 버퍼 크기를 담고 있는 변수의 주소 값 전달
* 
* int setsockopt(int sock, int level, int optname, const void* optval, socklen_t optlen)
* sock		: 옵션 변경을 위한 소켓 핸들 전달
* level		: 변경할 옵션의 프로토콜 레벨 전달
* optname	: 변경할 옵션의 이름 전달
* optval	: 변경할 옵션 정보를 저장한 버퍼의 주소 값 전달
* optlen	: 네 번째 매개변수의 크기 정보 전달
* 
* 3. SO_SENDBUF & SO_RECVBUF
* 소켓이 생성되면 입출력 버퍼가 생성된다. 이 버퍼들과 관련있는 소켓 옵션에 대해 소개하겠다.
* SO_RECVBUF는 입력 버퍼 크기와 관련된 옵션이고 SO_SENDBUF는 출력 버퍼의 크기와 관련이 있는 옵션이다.
* 이 두 옵션을 통해서 입출력 버퍼의 크기를 참조할 수 있을 뿐만 아니라 변경도 가능하다.
* 
* <<2. SO_REUSEADDR>>
* 이번에 설명하는 SO_REUSEADDR 옵션, 그리고 그와 관련된 Time-Wait 상태는 상대적으로 중요하다. 따라서 반드시 이해하고 넘어가자.
* 1) 주소 할당 에러
* 2) Time-Wait 상태
* 3) 주소의 재할당
* 
* optlen = sizeof(option);
* option = TRUE;
* setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
* 
* <<3. TCP_NODELAY>>
* 1. Nagle 알고리즘
* Nagle 알고리즘은 앞서 전송한 데이터에 대한 ACK 메시지를 받아야만, 다음 데이터를 전송하는 알고리즘이다.
* 기본적으로 TCP 소켓은 Nagle 알고리즘을 적용해서 데이터를 전송한다. 때문에, ACK가 수신될 때 까지 최대한 버퍼링을 해서 데이터를 전송한다.
* Nagle 알고리즘을 적용하지 않으면, 네트워크 트레픽에는 좋지 않은 영향을 미친다. 그러나 항상 좋은 것은 아니다.
* 용량이 큰 파일 데이터의 전송이 대표적인 예이다. 그러므로 상황에 따라 적절히 끄고 키자.
* 
* 2. Nagle 알고리즘의 중단
* int opt_val = 1;
* setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));
*/