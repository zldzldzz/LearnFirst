def create_key_matrix(key):
    """
    5x5 키 매트릭스를 생성한다.
    Z는 Q로 대체한다.
    """
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  # 알파벳 A-Z
    key = key.upper().replace("Z", "Q")  # Z를 Q로 변환
    seen = set()
    key_matrix = []

    for char in key + alphabet:
        if char not in seen:  # 중복된 문자는 제거
            key_matrix.append(char)
            seen.add(char)
        if len(key_matrix) == 25:  # 5x5 매트릭스가 완성되면 종료
            break

    return [key_matrix[i:i + 5] for i in range(0, 25, 5)]  # 5x5로 나누어 반환
def preprocess_message(message):
    """
    메시지를 전처리한다.
    1. Z를 Q로 변환.
    2. 같은 문자가 연속되면 X를 삽입.
    3. XX가 있으면 X 사이에 Q를 삽입.
    4. 홀수 길이면 X를 추가.
    """
    message = message.upper().replace("Z", "Q")  # Z를 Q로 변환
    result = []
    i = 0

    while i < len(message):
        char = message[i]
        if char.isalpha():  # 알파벳만 처리
            result.append(char)
            if i + 1 < len(message) and message[i] == message[i + 1]:
                result.append("X")  # 같은 문자 사이에 X 추가
            elif len(result) >= 2 and result[-1] == "X" and result[-2] == "X":
                result.insert(-1, "Q")  # XX 처리
        else:
            result.append(char)
        i += 1

    if len(result) % 2 == 1:  # 홀수 길이면
        result.append("X")  # 마지막에 X 추가

    return result

def find_position(char, key_matrix):
    for i, row in enumerate(key_matrix):
        if char in row:
            return i, row.index(char)
    raise ValueError(f"Character {char} not found in key matrix.")

def playfair_encrypt(plaintext, key):
    """
    플레이페어 암호화 알고리즘을 구현한다.
    특수문자는 건너뛰고 그대로 암호문에 포함한다.
    """
    key_matrix = create_key_matrix(key)  # 5x5 키 매트릭스 생성
    plaintext_pairs = preprocess_message(plaintext)  # 메시지를 전처리하여 문자 쌍 생성
    ciphertext = ""
    
    i = 0
    while i < len(plaintext_pairs):
        char1 = plaintext_pairs[i]
        # 특수문자나 알파벳이 아닌 문자는 그대로 추가하고 건너뜀
        if not char1.isalpha():
            ciphertext += char1
            i += 1
            continue
        
        # 두 번째 문자도 처리 (특수문자일 가능성 있음)
        if i + 1 < len(plaintext_pairs):
            char2 = plaintext_pairs[i + 1]
        else:
            char2 = 'X'  # 홀수 길이일 경우 X 추가
        
        if not char2.isalpha():
            ciphertext += char1  # char1은 처리 가능
            ciphertext += char2  # 특수문자는 그대로 추가
            i += 2
            continue
        
        # char1과 char2가 모두 알파벳인 경우 플레이페어 규칙 적용
        row1, col1 = find_position(char1, key_matrix)
        row2, col2 = find_position(char2, key_matrix)

        if row1 == row2:  # 같은 행에 있는 경우
            ciphertext += key_matrix[row1][(col1 + 1) % 5]
            ciphertext += key_matrix[row2][(col2 + 1) % 5]
        elif col1 == col2:  # 같은 열에 있는 경우
            ciphertext += key_matrix[(row1 + 1) % 5][col1]
            ciphertext += key_matrix[(row2 + 1) % 5][col2]
        else:  # 직사각형의 다른 꼭짓점
            ciphertext += key_matrix[row1][col2]
            ciphertext += key_matrix[row2][col1]
        
        i += 2  # 문자 쌍 처리 완료 후 다음으로 넘어감

    return ciphertext

def playfair_decrypt(ciphertext, key):
    """
    플레이페어 복호화 알고리즘을 구현한다.
    특수문자는 그대로 복호문에 포함한다.
    """
    key_matrix = create_key_matrix(key)
    plaintext = ""
    
    i = 0
    while i < len(ciphertext):
        char1 = ciphertext[i]
        # 특수문자 처리
        if not char1.isalpha():
            plaintext += char1  # 특수문자는 그대로 추가
            i += 1
            continue
        
        if i + 1 < len(ciphertext):
            char2 = ciphertext[i + 1]
        else:
            char2 = 'X'  # 홀수 길이 처리
        
        if not char2.isalpha():
            plaintext += char1  # char1 처리 후
            plaintext += char2  # char2 그대로 추가
            i += 2
            continue

        # char1과 char2가 모두 알파벳인 경우에만 플레이페어 복호화 적용
        row1, col1 = find_position(char1, key_matrix)
        row2, col2 = find_position(char2, key_matrix)

        if row1 == row2:  # 같은 행에 있는 경우
            plaintext += key_matrix[row1][(col1 - 1) % 5]
            plaintext += key_matrix[row2][(col2 - 1) % 5]
        elif col1 == col2:  # 같은 열에 있는 경우
            plaintext += key_matrix[(row1 - 1) % 5][col1]
            plaintext += key_matrix[(row2 - 1) % 5][col2]
        else:  # 직사각형의 다른 꼭짓점
            plaintext += key_matrix[row1][col2]
            plaintext += key_matrix[row2][col1]
        
        i += 2  # 문자 쌍 처리 완료

    # X와 Q 처리된 내용을 복원
    plaintext = plaintext.replace("XQ", "XX").rstrip("X")
    return plaintext.replace("Z", "Q")  # Z를 다시 Q로 복원


def beginner_encrypt(text, shift_key):
    """
    비기너 암호화: 문자열 키를 사용하여 입력 텍스트의 모든 문자(특수문자 포함)를 순환적으로 이동시킨다.
    """
    encrypted = []
    key_length = len(shift_key)  # 문자열 키의 길이
    
    for i, char in enumerate(text):
        base = ord(' ')  # 특수문자를 포함한 모든 ASCII 문자 처리
        shift = ord(shift_key[i % key_length].upper()) - ord('A')  # 키로부터 이동값 계산
        encrypted.append(chr((ord(char) - base + shift) % 95 + base))  # 95: ASCII 인쇄 가능 문자 범위
    return ''.join(encrypted)


def beginner_decrypt(text, shift_key):
    """
    비기너 복호화: 문자열 키를 사용하여 모든 문자(특수문자 포함)를 복호화한다.
    """
    decrypted = []
    key_length = len(shift_key)  # 문자열 키의 길이
    
    for i, char in enumerate(text):
        base = ord(' ')  # 특수문자를 포함한 모든 ASCII 문자 처리
        shift = ord(shift_key[i % key_length].upper()) - ord('A')  # 키로부터 이동값 계산
        decrypted.append(chr((ord(char) - base - shift) % 95 + base))  # 95: ASCII 인쇄 가능 문자 범위
    return ''.join(decrypted)


def display_key_matrix(key_matrix):
    """
    5x5 키 매트릭스를 보기 좋게 출력한다.
    """
    print("5x5 Key Matrix:")
    for row in key_matrix:
        print(" ".join(row))

def playfair_with_beginner_encrypt(plaintext, key, beginner_shift):
    """
    플레이페어 암호화 후 비기너 암호화를 추가로 진행한다.
    """
    # 1. 키 매트릭스를 출력
    key_matrix = create_key_matrix(key)
    display_key_matrix(key_matrix)

    # 2. 플레이페어 암호화
    playfair_ciphertext = playfair_encrypt(plaintext, key)
    print("\n플레이 페어 암호화 결과:", playfair_ciphertext)

    # 3. 비기너 암호화
    final_ciphertext = beginner_encrypt(playfair_ciphertext, beginner_shift)
    print("\n비기너 암호화 결과:", final_ciphertext)

    return final_ciphertext

def playfair_with_beginner_decrypt(ciphertext, key, beginner_shift):
    """
    비기너 복호화를 먼저 수행한 뒤 플레이페어 복호화를 진행한다.
    """
    # 1. 비기너 복호화
    playfair_ciphertext = beginner_decrypt(ciphertext, beginner_shift)
    print("\n비기너 복화화 결과:", playfair_ciphertext)

    # 2. 플레이페어 복호화
    plaintext = playfair_decrypt(playfair_ciphertext, key)
    print("\n(최종)플레이 페어 복호화 결과:", plaintext)

    return plaintext

# 테스트
key = "PLAYFAIREXAMPLE"
plaintext = "1q2w3e4r!rhkwpgkrltlfgdj"#QZ오류
beginner_shift = "key"

print("암호화 대상 값:", plaintext)
ciphertext = playfair_with_beginner_encrypt(plaintext, key, beginner_shift)
decrypted_text = playfair_with_beginner_decrypt(ciphertext, key, beginner_shift)
