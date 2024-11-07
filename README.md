# URL Downloader-----------------------------------------------------------------------------------

Этот проект представляет собой инструмент для загрузки изображений по URL-адресам, указанным в текстовом файле.
Удобен он тем, что при сохранении изобажения создаёт для неё имя и если название кода превышает 100 символов,
то по умолчанию переименовывает его на "Image". Также пронумеровывает изображения при наличии одинаковых имен.

## Установка и использование-----------------------------------------------------------------------

Для запуска программы следуйте следующей инструкции:

	1. Склонируйте репозиторий: git clone "https://github.com/DzhamaludinOsmanov/url_downloader"

	2. Откройте терминал (Bash/CMD/PS) (тут можно скачать Bash: https://git-scm.com/downloads) в склонированой репозитории
	
	3. Войдите в директорию - build/Release
	
	4. Введите команду (./url_downloader "/path/to/urls.txt" "/path/to/output_dir" <max_conc_download>), где:

		4.1. "/path/to/urls.txt" - путь к текстовому файлу со списком url-адресов
		
		4.2. "/path/to/output_dir" - путь в которую хотите сохранить изображения
		
		4.3. <max_conc_download> - количество одновременных загрузок