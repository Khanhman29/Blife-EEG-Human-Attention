
import requests
import playsound

with open("speak.txt", "r", encoding="UTF-8") as o:
    try:
        url = o.readline();
        res = requests.get(url)
        if res.status_code == 200:
            print("Đang phát âm thanh")
            with open("audio.mp3", 'wb') as f:
                f.write(res.content)
    
            playsound.playsound('audio.mp3', True)
        else:
            print("Không gọi được api đến âm thanh!!!")
    except:
        print("Lỗi thực thi api!")