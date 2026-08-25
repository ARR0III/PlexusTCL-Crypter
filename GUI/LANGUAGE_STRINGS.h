#ifdef PTCL_RUSSIAN_LANGUAGE
const wchar_t STR_PROGRAMM_NAME[]          = L"PlexusTCL Crypter 7.00 23AUG26 [RU]";

const wchar_t STR_ERASED_FILE_OK[]         = L"Файл назначения был уничтожен.";
const wchar_t STR_ENTER_8_TO_256[]         = L"Введите число от 8 до 256.";
const wchar_t STR_MEMORY_BLOCKED[]         = L"Ошибка выделения памяти!";
const wchar_t STR_WARNING_MSG[]            = L"Внимание!";
const wchar_t STR_ERROR_MSG[]              = L"Ошибка!";

const wchar_t STR_INPUT_FILENAME[]         = L"Файл для шифрования";
const wchar_t STR_OUTPUT_FILENAME[]        = L"Файл назначения";
const wchar_t STR_KEY_FILENAME[]           = L"Ключ шифрования";

const wchar_t STR_ERASED[]                 = L"Стереть";
const wchar_t STR_START[]                  = L"Старт";
const wchar_t STR_STOP[]                   = L"Стоп";
const wchar_t STR_OPEN[]                   = L"Открыть";
const wchar_t STR_CLOSE[]                  = L"Закрыть";
const wchar_t STR_KEYGEN[]                 = L"Генератор";

const wchar_t STR_EN[]                     = L"Зашифровать";
const wchar_t STR_DE[]                     = L"Расшифровать";

const wchar_t STR_CRYPT_ALG[]              = L"Алгоритм шифрования";
const wchar_t STR_KEY_LENGTH[]             = L"Длина ключа шифрования";
const wchar_t STR_KEY_OR_KEY_FILENAME[]    = L"Пароль или путь к ключевому файлу";

const wchar_t STR_STOP_PROCESS[]           = L"Прервать операцию?";
const wchar_t STR_INPUT_FILENAME_EMPTY[]   = L"Имя обрабатываемого файла не введено!";
const wchar_t STR_OUTPUT_FILENAME_EMPTY[]  = L"Имя файла назначения не введено!";
const wchar_t STR_PASS_OR_KEYFILE_EMPTY[]  = L"Пароль или имя ключевого файла не введены!";
const wchar_t STR_IN_OUT_FILENAME_EQUAL[]  = L"Имена обрабатываемого файла и файла назначения совпадают!";
const wchar_t STR_OUT_KEY_FILENAME_EQUAL[] = L"Имена обрабатываемого файла и ключевого файла совпадают!";
const wchar_t STR_IN_KEY_FILENAME_EQUAL[]  = L"Имена файла назначения и ключевого файла совпадают!";
const wchar_t STR_IN_FILENAME_NFOUND[]     = L"Файл для обработки не существует!";
const wchar_t STR_CHANGE_OUT_FILENAME[]    = L"Измените имя файла назначения!";

const wchar_t STR_CIPHER_NOT_ENTER[]       = L"Алгоритм шифрования не был выбран!";
const wchar_t STR_KEY_SIZE_NOT_ENTER[]     = L"Длина ключа шифрования не была выбрана!";
const wchar_t STR_OPERATION_NOT_ENTER[]    = L"Операция не была выбрана!";

const wchar_t STR_OK[]                          = L"Файл успешно обработан.";
const wchar_t STR_READ_FILE_NOT_OPEN[]          = L"Файл для обработки не был открыт!";
const wchar_t STR_WRITE_FILE_NOT_OPEN[]         = L"Файл назначения не был открыт!";
const wchar_t STR_READ_FILE_ERROR[]             = L"Ошибка чтения из файла!";
const wchar_t STR_WRITE_FILE_ERROR[]            = L"Ошибка записи в файл!";
const wchar_t STR_STREAM_IN_CLOSE_ERROR[]       = L"Поток ввода не был закрыт!";
const wchar_t STR_STREAM_OUT_CLOSE_ERROR[]      = L"Поток вывода не был закрыт!";
const wchar_t STR_SIZE_FILE_EMPTY[]             = L"Файл для обработки пуст!";
const wchar_t STR_SIZE_FILE_VERY_BIG[]          = L"Размер файла для обработки превышает 4 ЭиБ!";
const wchar_t STR_OPERATION_BREAK[]             = L"Операция была отменена.";
const wchar_t STR_DATA_FROM_FILE_SMALL[]        = L"Данных в ключевом файле недостаточно.";
const wchar_t STR_CANNOT_ALLOCATE_MEMORY[]      = L"Ошибка выделения памяти!";
const wchar_t STR_KEY_LEN_INCORRECT[]           = L"Длина пароля должна быть от 8 до 256 символов.";
const wchar_t STR_KEY_FILE_NOT_OPENNED[]        = L"Ключевой файл пуст или не был открыт.";

const wchar_t STR_SIZE_DECRYPT_FILE_INCORRECT[] =
  L"Размер файла для расшифровки некорректен!\n"
  L"Обрабатываемый файл ранее был зашифрован?";
const wchar_t STR_ERASED_FILE_QUES[] =
  L"Вы уверены что хотите уничтожить файл для обработки?\n"
  L"Стертые данные будет невозможно восстановить.";
const wchar_t STR_ERROR_ERASED_FILE[] =
  L"Ошибка перезаписи файла!\n"
  L"Возможно файл не существует или защищен от записи.";
const wchar_t STR_OUT_FILE_FOUND[] =
  L"Файл назначения существует! Старые данные будут утеряны!\n"
  L"Вы уверены что хотите перезаписать его?";
const wchar_t STR_CONTROL_SUM_FILE_ERROR[] = 
  L"Контрольная сумма файла НЕ совпадает с ожидаемой!\n"
  L"Возможно файл поврежден или был использован неправильный ключ!";

const char * STR_STATUS                 = "Состояние...";
const char * STR_ENCRYPT_FILE_PROC      = "%s: %s; Обработано: ";
const char * STR_PROGRESS_BAR_MKS       = "%4.2f %s / %4.2f %s; Прогресс: %3d %%";
#else /* PTCL_ENGLISH_LANGUAGE */
const wchar_t STR_PROGRAMM_NAME[] = L"PlexusTCL Crypter 7.00 23AUG26 [EN]";

const wchar_t STR_ERASED_FILE_OK[] = L"The destination file was destroyed!";
const wchar_t STR_ENTER_8_TO_256[] = L"Enter a number between 8 and 256!";
const wchar_t STR_MEMORY_BLOCKED[] = L"Memory allocation error!";
const wchar_t STR_WARNING_MSG[] = L"Attention!";
const wchar_t STR_ERROR_MSG[] = L"Error!";

const wchar_t STR_INPUT_FILENAME[]  = L"Source file";
const wchar_t STR_OUTPUT_FILENAME[] = L"Destination file";
const wchar_t STR_KEY_FILENAME[] = L"Encryption key";

const wchar_t STR_ERASED[] = L"Erase";
const wchar_t STR_START[] = L"Start";
const wchar_t STR_STOP[] = L"Stop";
const wchar_t STR_OPEN[] = L"Open";
const wchar_t STR_CLOSE[] = L"Close";
const wchar_t STR_KEYGEN[] = L"Generator";

const wchar_t STR_EN[] = L"Encrypt";
const wchar_t STR_DE[] = L"Decrypt";

const wchar_t STR_CRYPT_ALG[] = L"Encryption algorithm";
const wchar_t STR_KEY_LENGTH[] = L"Encryption key length";
const wchar_t STR_KEY_OR_KEY_FILENAME[] = L"Password or path to key file";

const wchar_t STR_STOP_PROCESS[] = L"Abort the operation?";
const wchar_t STR_INPUT_FILENAME_EMPTY[] = L"The name of the processed file has not been entered!";
const wchar_t STR_OUTPUT_FILENAME_EMPTY[] = L"The name of the destination file has not been entered!";
const wchar_t STR_PASS_OR_KEYFILE_EMPTY[] = L"The password or the name of the key file has not been entered!";
const wchar_t STR_IN_OUT_FILENAME_EQUAL[] = L"The names of the processed file and the destination file match!";
const wchar_t STR_OUT_KEY_FILENAME_EQUAL[] = L"The names of the processed file and the key file match!";
const wchar_t STR_IN_KEY_FILENAME_EQUAL[] = L"The names of the destination file and the key file match!";
const wchar_t STR_IN_FILENAME_NFOUND[] = L"The file for processing does not exist!";
const wchar_t STR_CHANGE_OUT_FILENAME[] = L"Change the name of the destination file!";

const wchar_t STR_CIPHER_NOT_ENTER[] = L"No encryption algorithm has been selected!";
const wchar_t STR_KEY_SIZE_NOT_ENTER[] = L"Encryption key length was not selected!";
const wchar_t STR_OPERATION_NOT_ENTER[] = L"Operation was not selected!";

const wchar_t STR_OK[] = L"File processed successfully.";
const wchar_t STR_READ_FILE_NOT_OPEN[] = L"File for processing was not open!";
const wchar_t STR_WRITE_FILE_NOT_OPEN[] = L"Destination file was not open!";
const wchar_t STR_READ_FILE_ERROR[] = L"Error reading from file!";
const wchar_t STR_WRITE_FILE_ERROR[] = L"Error writing to file!";
const wchar_t STR_STREAM_IN_CLOSE_ERROR[] = L"Input stream was not closed!";
const wchar_t STR_STREAM_OUT_CLOSE_ERROR[] = L"Output stream was not closed!";
const wchar_t STR_SIZE_FILE_EMPTY[] = L"File for processing is empty!";
const wchar_t STR_SIZE_FILE_VERY_BIG[] = L"The file to process exceeds 4 EiB!";
const wchar_t STR_OPERATION_BREAK[] = L"The operation was canceled.";
const wchar_t STR_DATA_FROM_FILE_SMALL[] = L"There is not enough data in the key file.";
const wchar_t STR_CANNOT_ALLOCATE_MEMORY[] = L"Memory allocation error!";
const wchar_t STR_KEY_LEN_INCORRECT[] = L"Password length must be between 8 and 256 characters.";
const wchar_t STR_KEY_FILE_NOT_OPENNED[] = L"The key file is empty or has not been opened.";

const wchar_t STR_SIZE_DECRYPT_FILE_INCORRECT[] =
  L"The size of the file to decrypt is incorrect!\n"
  L"Was the file to be processed previously encrypted?";
const wchar_t STR_ERASED_FILE_QUES[] =
  L"Are you sure you want to delete the file to process?\n"
  L"Erased data cannot be recovered!";
const wchar_t STR_ERROR_ERASED_FILE[] =
  L"Error overwriting file!\n"
  L"The file may not exist or is write-protected!";
const wchar_t STR_OUT_FILE_FOUND[] =
  L"The destination file exists! Old data will be lost!\n"
  L"Are you sure you want to overwrite it?";
const wchar_t STR_CONTROL_SUM_FILE_ERROR[] =
  L"The file checksum does NOT match the expected one!\n"
  L"The file may be corrupted or the wrong key was used!";

const char * STR_STATUS            = "Status...";
const char * STR_ENCRYPT_FILE_PROC = "%s: %s; Processed: ";
const char * STR_PROGRESS_BAR_MKS  = "%4.2f %s / %4.2f %s; Progress: %3d %%";
#endif
