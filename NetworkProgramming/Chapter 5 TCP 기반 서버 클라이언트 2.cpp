/*
* <<1. 에코 클라이언트의 완벽 구현>>
* 
* 1. 에코 서버는 문제가 없고, 에코 클라이언트만 문제가 있나요?
* 에코 서버:
* while((str_len = recv(clnt_sock, message, BUF_SIZE) != )
*		send(clnt_sock, message, str_len);
* 
* 이코 클라이언트:
* send(sock, message, strlen(message));
* str_len = recv(sock, message, BUF_SIZE - 1);
* 
* 뭐가 문제인지 알겠는가? 서버는 계쏙 받는데 클라이언트는 한번만 받는다는 것이다.
* 
* 2. 에코 클라이언트의 해결책
* 해결책은 매우 쉽다. 클라이언트가 수신해야할 데이터의 크기를 미리 알고 있기 때문에 전송한 데이터 크기를 읽을 만큼 recv 함수를 호출하면 된다.
* 
* recv_len = 0;
* while(recv_len < str_len){
*	recv_cnt = recv(sock, &message[recv_len], BUF_SIZE - 1);
*	recv_len += recv_cnt;
* }
* 
* 3. 에코 클라이언트 이외의 경우에는? 어플리케이션 프로토콜의 정의
* 에코 클라이언트의 경우에는 수신할 데이터의 크기를 이전에 파악할 수 있지만, 이것이 불가능한 경우가 훨씬 많다. 그렇다면 이런 경우는 어떻게 해야할까?
* 이런 경우에 필요한 것이 어플리케이션 프로토콜이다!
* 
* "Q가 전달되면 연결을 종료한다"
* 
* 위와 같은 약속이 있으면 해결이 가능하다.
* 
* <<2. TCP의 이론적인 이야기>>
* 이쯤에서 TCP의 얘기를 마무리 지으려 한다. 아쉬우니 이론적인 얘기를 하나 하겠다.
* 
* 1. TCP 내부 동작 원리 1: TCP 소켓의 입출력 버퍼
* TCP 소켓에는 입출력의 경계가 없음을 말했다. 그런데 이 현상에 의문이 들 수 있다.
* 
*			(recv)입력 버퍼	 <──────	출력 버퍼(send)
* 서버														   클라이언트
*			(send)출력 버퍼	 ──────>  입력 버퍼(recv)
* 
* 1) 입출력 버퍼는 TCP 소켓 각각에 대해 별도로 존재한다.
* 2) 입출력 버퍼는 소켓 생성시 자동으로 생성된다.
* 3) 소켓을 닫아도 출력 버퍼에 남아있는 데이터는 계속해서 전송이 이뤄진다.
* 4) 소켓을 닫으면 입력 버퍼에 남아있는 데이터는 소멸된다.
* 
* 2. TCP 내부 동작 원리 2: 상대 소켓과의 연결
* TCP 소켓의 생성에서 소멸의 과정까지 거치게 되는 일을 크게 나누면, 다음 세 가지로 나눌 수 있다.
* 
* 1) 상대 소켓과의 연결
* 2) 데이터 송수신
* 3) 상대 소켓과의 연결 종료
* 
* 그렇다면 상대 소켓과의 연결이 어떻게 이뤄지는지 설명하겠다.
* 
* 호스트 A                      호스트 B
*				[shake 1]
*				SEQ : 1000
*				ACK : 
*				─────>
* 
*				[shake 2]
*				SEQ : 2000
*				ACK : 1001
*				<─────
* 
*				[shake 3]
*				SEQ : 1001
*				ACK : 2001
*				─────>
* 
* 3. TCP 내부 동작 원리 3: 상대 소켓과의 데이터 송수신
* 처음 진행한 Three-HandShaking을 통해서 소켓 연결을 했다.
* 데이터 송수신 과정은 다음과 같다.
* 
* 호스트 A						호스트 B
* 
*				SEQ : 1200
*				100 byte data
*				──────>
* 
*				ACK : 1301
*				<──────
* 
*				SEQ : 1301
*				100 byte data
*				──────>
* 
*				ACK : 1402
*				<──────
* 
* 4. TCP 내부 동작 원리 4: 상대 소켓과의 연결 종료
* 
* 호스트 A						호스트 B
* 
*				[shake 1]
*				FIN
*				SEQ : 5000
*				ACK : 
*				─────>
* 
*				[shake 2]
*				ACK
*				SEQ : 7500
*				ACK : 5001
*				<─────
* 
*				[shake 3]
*				FIN
*				SEQ : 7501
*				ACK	: 5001
*				<─────
* 
*				[shake 4]
*				ACK
*				SEQ : 5001
*				ACK : 7502
*				<─────
*
*/			
