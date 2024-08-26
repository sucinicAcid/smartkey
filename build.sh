# vcan 인터페이스 생성 함수
function create_vcan_interface() {
    local name=$1
    
    # 인터페이스가 존재하면 삭제
    if ip link show | grep -q "$name"; then
        echo "$name 인터페이스가 이미 존재합니다. 삭제 후 재생성합니다."
        sudo ip link set $name down
        sudo ip link delete $name
    fi
    
    # 새로운 인터페이스 생성
    echo "$name 인터페이스를 생성합니다."
    sudo ip link add dev $name type vcan
}


# vcan 인터페이스 생성 (기존에 존재하면 삭제 후 재생성)===========
create_vcan_interface "vcanMainECU"
create_vcan_interface "vcanLockDoorECU"


echo "인터페이스를 활성화합니다."
sudo ifconfig vcanMainECU up
sudo ifconfig vcanLockDoorECU up


echo "can 게이트웨이를 설정합니다."
sudo cangw -A -s vcanLockDoorECU -d vcanMainECU -e
sudo cangw -A -s vcanMainECU -d vcanLockDoorECU -e

echo -e "vcan 인터페이스 설정 완료!\n"
# ============================================================


# 소스 파일 컴파일=============================================
echo "소스 파일을 컴파일합니다."
gcc -o main main.c server.c can.c
gcc -o lockDoorECU lockDoorECU.c server.c can.c

echo -e "컴파일 완료!\n"
# ============================================================


# 실행========================================================
echo -e "실행 시작.\n"
./main
# ============================================================