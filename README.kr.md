# EZIPC - Easy & Lightweight Interprocessing Library

프로세스 간 통신 기능을 구현하는 경량 라이브러리입니다.

# 상세

- IPC를 시작하기 위해, 하나의 프로세스에서 `ipc_server` 클래스 인스턴스를 생성해야 합니다.
  - `ipc_server<tcp_server_t>`로 서버 모드의 IPC 인스턴스를 열 수 있습니다.
  - 각각의 `ipc_server` 인스턴스는 하나의 인터프로세스 네트워크에 대응됩니다.
- 인터프로세스 기능을 사용하려는 모든 프로세스는 `ipc_client` 인스턴스를 생성해 `ipc_server`에 접속합니다.
  - `ipc_client<tcp_client_t>`로 클라이언트 모드의 IPC 인스턴스를 생성합니다. 
    - 클라이언트 IPC는 포트 번호 및 IP 주소(선택)로 네트워크 내의 IPC 서버 인스턴스에 접속을 시도합니다.
- 활성화된 `ipc_client`에서 ascii 문자열 형식의 변수 ID로 새 메모리를 할당할 수 있습니다. 할당된 메모리는 모든 클라이언트 사이에 동기화됩니다. 그러나, `sync_policy`에 따라 동기화 방식이 달라집니다.
  - `ipc_client::allocate(string_view key, size_t size, sync_policy)->ipc_ptr<sync_policy>`로 새 메모리 할당
  - `ipc_ptr`은 공유 메모리에 읽기 및 쓰기를 제공합니다. 
    - `sync_policy_request` 설정 시 클라이언트에는 데이터의 사본이 저장되지 않으며, `ipc_ptr` 데이터 동기화는 `ipc_ptr`에 읽기를 시도하는 시점까지 지연됩니다. 
    - `sync_policy_lazy` 클라이언트에 데이터의 사본을 저장하고, 
    - `sync_policy_poll` 데이터를 항상 최신 상태로 유지합니다. read() 함수는 즉시 값을 반환합니다.
    - 기본적으로 클라이언트 내에 존재하는 policy중 가장 높은 것을 따릅니다. 예를 들어 request 포인터와 poll 포인터가 같이 있다면, request 포인터에 읽기 요청을 하는 즉시 값을 반환합니다.
  - `ipc_ptr`의 레퍼런스 카운터는 서버에 저장됩니다. `allocate()`에서 반환된 `ipc_ptr`은 레퍼런스 카운트를 자동으로 관리합니다. 
    - 동시에 두 프로세스에서 같은 변수 ID에 `allocate()`를 실행할 수 없습니다. 항상 하나의 프로세스가 먼저 `allocate()`로 메모리를 생성하면, 다른 프로세스에서는 `watch(string_view key, sync_policy)->ipc_ptr<sync_policy>`로 포인터를 획득해야 합니다. \
    `watch` 함수는 해당 키의 변수가 아직 생성되지 않았다면 nullptr을 반환하며, 이미 모든 클라이언트에서 dispose된 경우 예외를 던집니다.
    - 시간차에 의한 의도하지 않은 예외를 방지하기 위해, 서버에서는 reference count가 0이 되더라도 해당 키가 invalidate될 때까지의 지연 시간을 설정할 수 있습니다. 이 경우 모든 클라이언트에서 해당 `ipc_ptr`이 invalidate 되더라도 일정 시간 전까지는 `watch()`를 호출할 수 있습니다.
    - 서버에서 dispose되지 않는 메모리를 생성하는 것도 가능합니다.
    - `ipc_client` 인스턴스가 있어도, `ipc_ptr`이 없는 항목에 대해서는 동기화가 이루어지지 않습니다.

# 확장

- `ipc_server`는 다음의 모듈들을 `(server_connection_manager, codec, allocator)` 필요로 합니다.
  - `server_connection_manager`는 접속된 클라이언트의 목록을 관리합니다. 각 클라이언트는 8바이트 ID로 관리합니다. 다음의 함수를 구현해야 합니다.
    - `assign_read_callback(array_view<char> data, client_id_t sender)`
    - `assign_write_callback(array_view<char> data, client_id_t sender)`
    - `write_to_async(array_view<client_id_t> to, array_view<char> data)`
      - `to` 목록에 든 클라이언트들에 `data`를 일괄 전송합니다. 
      - 전송이 종료되면 `assign_write_callback()`에서 
- 

# TODO
- C#, python 래퍼 구현