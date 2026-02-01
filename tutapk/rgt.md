# Lorer.exe - Руководство пользователя (README)

## 📋 Содержание
1. [Общее описание](#общее-описание)
2. [Требования к системе](#требования-к-системе)
3. [Установка и настройка](#установка-и-настройка)
4. [Использование](#использование)
5. [Конфигурация офсетов](#конфигурация-офсетов)
6. [Настройка BlueStacks 5](#настройка-bluestacks-5)
7. [Решение проблем](#решение-проблем)
8. [Технические детали](#технические-детали)

---

## 📖 Общее описание

**Lorer.exe** (ранее Standoff2Loader.exe) - это инструмент для инъекции модуля в процесс эмулятора BlueStacks 5 с игрой Standoff 2. Инструмент предназначен для educational purposes и позволяет взаимодействовать с памятью игры через инжектированную DLL.

### Состав пакета
- `lorer.exe` - основной исполняемый файл (загрузчик)
- `Standoff2Core.dll` - ядро с функциями чтения/записи памяти

---

## 💻 Требования к системе

### Обязательные требования:
- **ОС**: Windows 10/11 (64-bit)
- **Эмулятор**: BlueStacks 5 (Nougat 32-bit или Pie 64-bit)
- **Игра**: Standoff 2 v0.37.0 (или другая версия с обновлёнными офсетами)
- **Права**: Администратор для запуска загрузчика

### Рекомендуемые требования:
- **RAM**: минимум 8 ГБ (для работы BlueStacks)
- **Процессор**: 4+ ядер
- **Видеокарта**: NVIDIA GTX 1050 или лучше (для работы с BlueStacks)
- **Место на диске**: минимум 5 ГБ свободного места

---

## 🚀 Установка и настройка

### Шаг 1: Подготовка BlueStacks 5

1. **Скачайте и установите BlueStacks 5**
   - Официальный сайт: https://www.bluestacks.com/
   - При установке выберите версию "Nougat 32-bit" или "Pie 64-bit"

2. **Настройте эмулятор**
   ```
   Settings → Advanced → Enable Android Debug Bridge: ON
   Settings → Graphics → Graphics Engine: DirectX (опционально OpenGL)
   Settings → Performance → High Performance
   ```

3. **Установите Standoff 2**
   - Откройте Google Play Store в BlueStacks
   - Найдите и установите Standoff 2
   - Запустите игру один раз, чтобы завершить начальную загрузку

### Шаг 2: Развертывание файлов

1. **Распакуйте архив** в отдельную папку (например: `C:\Lorer\`)

2. **Убедитесь, что в папке находятся файлы:**
   ```
   /Lorer/
   ├── lorer.exe              ← Главный загрузчик
   ├── Standoff2Core.dll      ← Ядро модуля
   └── rgt.md                 ← Этот файл
   ```

3. **Важно!** Не переименовывайте файлы и не изменяйте структуру папок!

---

## 🎮 Использование

### Пошаговая инструкция:

#### 1. Запуск BlueStacks и игры
```
1. Запустите BlueStacks 5
2. Запустите Standoff 2 в эмуляторе
3. Дождитесь полной загрузки игры до главного меню
4. Убедитесь, что процесс HD-Player.exe активен (можно проверить в Диспетчере задач)
```

#### 2. Запуск загрузчика
```
1. Нажмите правой кнопкой мыши на lorer.exe
2. Выберите "Запуск от имени администратора"
3. Подтвердите действие в UAC (если появится запрос)
```

#### 3. Инъекция модуля
```
Если всё настроено правильно, вы увидите:
✓ SUCCESS: "Модуль успешно инициализирован. Используйте назначенные клавиши для управления."

Если возникла ошибка:
✗ ERROR: "Процесс эмулятора не найден. Убедитесь, что BlueStacks 5 запущен."
   → Решение: Запустите BlueStacks 5 и Standoff 2

✗ ERROR: "Не удалось инициализировать модуль в целевом процессе."
   → Решение: Проверьте права администратора, отключите антивирус
```

---

## ⚙️ Конфигурация офсетов

### Что такое офсеты?

Офсеты (offsets) - это адреса в памяти игры, которые определяют положение важных данных (здоровье, позиция игрока и т.д.). Они зависят от версии игры.

### Текущие офсеты для Standoff 2 v0.37.0:

```cpp
// PlayerManager
PlayerManager_LocalPlayer       = 0x68
PlayerManager_PlayerList        = 0x38

// PlayerController
PlayerController_Health         = 0x150
PlayerController_Team           = 0x154
PlayerController_MovementController = 0x98

// MovementController
MovementController_TranslationData = 0xB0

// TranslationData
TranslationData_Position        = 0x1C
TranslationData_Rotation        = 0x28
TranslationData_Velocity        = 0x34
```

### Цепочка для получения позиции игрока:
```
Unity Base Address
  └─> PlayerManager (offset 0x10-0x18 - нужно найти!)
       └─> LocalPlayer (offset 0x68)
            └─> MovementController (offset 0x98)
                 └─> TranslationData (offset 0xB0)
                      └─> Position Vector3 (offset 0x1C)
```

### Как обновить офсеты для другой версии игры:

#### Метод 1: Через Cheat Engine (рекомендуется)

1. **Скачайте Cheat Engine** с https://www.cheatengine.org/

2. **Подключитесь к процессу**
   ```
   - Запустите Cheat Engine как администратор
   - Нажмите на иконку компьютера (Process List)
   - Выберите HD-Player.exe
   ```

3. **Найдите PlayerManager**
   ```
   - Поиск: "pointer scan for value" с известным значением здоровья
   - Используйте поиск "Unknown initial value" → "Decreased value" (получение урона)
   - Когда найдёте указатель на PlayerController, перейдите по структуре к PlayerManager
   ```

4. **Определите офсеты**
   ```
   - Используйте "Structure Dissect" для анализа классов
   - Сравнивая структуры разных игроков, найдёте офсеты полей
   - Запишите новые офсеты для обновления
   ```

#### Метод 2: Через Il2CppDumper + dnSpy

1. **Скачайте Il2CppDumper** с GitHub
2. **Дампните game.unity3d/libil2cpp.so**
3. **Откройте dump.cs** в dnSpy
4. **Найдите классы PlayerManager, PlayerController, TranslationData**
5. **Используйте offsetof макросы** для вычисления офсетов

### Обновление офсетов в исходном коде:

Если у вас есть доступ к исходному коду:

1. Откройте файл `src/core/offsets.h`
2. Измените значения констант под новую версию:
   ```cpp
   constexpr uintptr_t PlayerController_Health = 0xNEW_OFFSET;
   ```
3. Перекомпилируйте с помощью `build.bat` (Windows) или `build.sh` (Linux)

---

## 🐉 Настройка BlueStacks 5

### Оптимальные настройки для стабильной работы:

#### Настройки производительности:
```
Settings → Performance:
├─ CPU Allocation: 4+ cores
├─ Memory Allocation: 4096 MB (4GB) или выше
├─ Graphics Renderer: DirectX 11 (для Windows)
└─ FPS: 60 или High Performance
```

#### Настройки graphics:
```
Settings → Graphics:
├─ Graphics Engine: DirectX (или OpenGL, если DirectX работает нестабильно)
├─ Interface Renderer: OpenGL
├─ Enable Android Debug Bridge: ON
└─ Disable Frame Rate Limit: OFF
```

#### Настройки advanced:
```
Settings → Advanced:
├─ Rooting: Enable
├─ Google Analytics: Disable (для приватности)
└─ Instance Type: Pie 64-bit (рекомендуется)
```

### Важные примечания:
- ❌ НЕ используйте Hyper-V (проблемы с инъекцией)
- ❌ НЕ используйте VirtualBox-based BlueStacks
- ✅ Используйте Nougat 32-bit или Pie 64-bit версии
- ✅ Закройте другие эмуляторы перед запуском

---

## 🔧 Решение проблем

### Проблема: "Процесс эмулятора не найден"

**Причины:**
- BlueStacks не запущен
- Неверное название процесса (должно быть HD-Player.exe)
- Используется старая версия BlueStacks (4 вместо 5)

**Решения:**
```
1. Проверьте в Диспетчере задач (Ctrl+Shift+Esc), что процесс HD-Player.exe активен
2. Если используете BlueStacks 4 → переустановите на BlueStacks 5
3. Перезапустите BlueStacks от имени администратора
4. Убедитесь, что запущен Standoff 2 в эмуляторе
```

---

### Проблема: "Не удалось инициализировать модуль в целевом процессе"

**Причины:**
- Недостаточно прав (не запущен от администратора)
- Антивирус блокирует инъекцию
- Защита Windows Defender (Exploit Guard)

**Решения:**
```
1. Обязательно запускайте lorer.exe от имени администратора
2. Добавьте папку с lorer.exe в исключения антивируса
3. Отключите временно:
   - Windows Defender Real-time protection
   - Exploit Protection (System Settings → Security & Privacy → Exploit protection)
4. Проверьте наличие Standoff2Core.dll в той же папке, что и lorer.exe
```

---

### Проблема: Игровой краш после инъекции

**Причины:**
- Неверные офсеты для версии игры
- Конфликт с другими модами/читами
- Несовместимость версии игры

**Решения:**
```
1. Проверьте версию Standoff 2 (в правом нижнем углу меню)
2. Обновите офсеты в src/core/offsets.h под вашу версию
3. Перезапустите BlueStacks и попробуйте снова
4. Удалите/отключите другие моды для игры
```

---

### Проблема: Antivirus блокирует файлы

**Причины:**
- Инъекция DLL классифицируется как потенциально нежелательная программа

**Решения:**
```
Windows Defender:
1. Параметры → Защита от вирусов и угроз
2. Управление настройками
3. Исключения → Добавить исключение → Папка
4. Выберите папку с lorer.exe

Kaspersky / Norton / McAfee:
1. Настройки → Защита → Исключения
2. Добавьте папку с lorer.exe и сам файл lorer.exe
3. Отключите "Self-Defense" временно
```

---

### Проблема: Функции не работают/выдают неверные данные

**Причины:**
- Офсеты устарели
- Игра обновилась
- Неправильная структура памяти

**Решения:**
```
1. Проверьте версию игры
2. Перепроверьте офсеты через Cheat Engine
3. Сравните с текущими офсетами для v0.37.0:
   - PlayerController_Health должен быть 0x150
   - Если отличается → обновите офсеты
```

---

## 📚 Технические детали

### Архитектура системы:

```
Windows OS
  └─> lorer.exe (Loader)
        ├─> Поиск процесса HD-Player.exe
        ├─> Получение привилегий SE_DEBUG_NAME
        ├─> Инъекция Standoff2Core.dll в процесс
        └─> Очистка и выход

BlueStacks 5 (HD-Player.exe)
  └─> Standoff 2 (Android APK)
        └─> Unity Engine
              └─> libunity.so / UnityPlayer.dll
                    └─> Игровая память (PlayerManager, PlayerController...)
                          └─> Standoff2Core.dll (инжектирован)
                                ├─> MemoryManager (чтение/запись памяти)
                                ├─> Offsets (адреса структур)
                                └─> Hooks (перехват вызовов)
```

### Принцип работы:

1. **Loader (lorer.exe)**
   - Использует `CreateToolhelp32Snapshot` для поиска процесса
   - Получает `SE_DEBUG_NAME` привилегию через `AdjustTokenPrivileges`
   - Выделяет память в целевом процессе через `VirtualAllocEx`
   - Записывает путь к DLL через `WriteProcessMemory`
   - Создаёт удалённый поток с `CreateRemoteThread` (вызывает LoadLibraryW)

2. **Core (Standoff2Core.dll)**
   - Находит Unity базовый адрес через `EnumProcessModules`
   - Использует офсеты для навигации по структурам игры
   - Читает/пишет память через `ReadProcessMemory` / `WriteProcessMemory`
   - Может устанавливать хуки (например, на Present для ESP)

3. **Манипуляция памятью**
   - Все адреса вычисляются динамически через `GetDynamicAddress`
   - Используются цепочки офсетов: `[base, offset1, offset2, ...]`
   - Защита страниц памяти меняется через `VirtualProtectEx`

### Ключевые структуры:

```cpp
struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };
struct Matrix4x4 { float m[16]; };

struct PlayerController {
    int32_t health;         // offset 0x150
    int32_t team;           // offset 0x154
    uint8_t isAlive;        // offset 0x158
    MovementController* movement; // offset 0x98
    // ... другие поля
};

struct MovementController {
    TranslationData* translation; // offset 0xB0
};

struct TranslationData {
    Vector3 position;       // offset 0x1C
    Quaternion rotation;     // offset 0x28
    Vector3 velocity;       // offset 0x34
};
```

---

## ⚠️ Отказ от ответственности (Disclaimer)

### Важно знать:

1. **Educational Use Only**
   - Данный инструмент создан исключительно для образовательных целей
   - Использование для читов/модификаций игры нарушает Terms of Service
   - Автор не несёт ответственности за последствия использования

2. **Risks**
   - Риск бана аккаунта в игре
   - Риск блокировки устройства/эмулятора
   - Возможные проблемы с антивирусным ПО
   - Потенциальная нестабильность работы игры

3. **Legal Disclaimer**
   - Обратный инжиниринг и модификация программного обеспечения могут нарушать законы вашей страны
   - Пользователь несёт полную ответственность за использование инструмента

4. **Privacy**
   - Инструмент не собирает и не передаёт никаких пользовательских данных
   - Весь код является open-source и может быть аудирован

---

## 📞 Поддержка и обновления

### Где получить помощь:
- GitHub Issues (если репозиторий публичный)
- Форумы по реверс-инжинирингу (UnknownCheats, GuidHacking)
- Discord сообщества по Unity реверс-инжинирингу

### Обновление офсетов:
- Следите за обновлениями игры Standoff 2
- Используйте Il2CppDumper после каждого крупного обновления
- Ведите собственную базу офсетов для разных версий

---

## 📝 История версий

### v1.0.0 (Текущая)
- Первая публичная версия
- Поддержка Standoff 2 v0.37.0
- Базовые функции чтения/записи памяти
- Автоматическая инъекция в BlueStacks 5

---

## 🔗 Полезные ссылки

### Инструменты:
- [Cheat Engine](https://www.cheatengine.org/) - Поиск офсетов
- [Il2CppDumper](https://github.com/Perfare/Il2CppDumper) - Дампинг Unity игр
- [dnSpy](https://github.com/dnSpy/dnSpy) - Декомпиляция .NET
- [x64dbg](https://x64dbg.com/) - Отладчик для Windows

### Материалы:
- [Unity Memory Layout](https://docs.unity3d.com/Manual/class-ScriptableObject.html)
- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [BlueStacks Documentation](https://support.bluestacks.com/hc/en-us)

---

## ✅ Контрольный список перед использованием

Перед запуском убедитесь, что:

- [ ] Установлен BlueStacks 5 (не 4!)
- [ ] BlueStacks запущен и работает
- [ ] Standoff 2 запущен в эмуляторе
- [ ] Процесс HD-Player.exe активен в диспетчере задач
- [ ] lorer.exe и Standoff2Core.dll в одной папке
- [ ] Папка добавлена в исключения антивируса
- [ ] Запуск lorer.exe будет производиться от имени администратора
- [ ] Вы понимаете риски использования инструмента

---

**Удачи в использовании! 🎮**

*При возникновении проблем обратитесь к разделу "Решение проблем" выше.*
