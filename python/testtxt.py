def caesar_cipher(text, shift, mode='encrypt'):
    result = ""
    
    # 암호화 또는 복호화 모드 설정
    if mode == 'decrypt':
        shift = -shift
    
    for char in text:
        if char.isalpha():
            # 대문자와 소문자 구분
            start = ord('A') if char.isupper() else ord('a')
            # 시저 암호 공식 적용
            result += chr((ord(char) - start + shift) % 26 + start)
        else:
            # 알파벳이 아닌 경우 그대로 추가
            result += char
            
    return result

# 테스트
plain_text = "Hello, World!"
shift_value = 3

# 암호화
encrypted_text = caesar_cipher(plain_text, shift_value, mode='encrypt')
print(f"Encrypted: {encrypted_text}")

# 복호화
decrypted_text = caesar_cipher(encrypted_text, shift_value, mode='decrypt')
print(f"Decrypted: {decrypted_text}")
