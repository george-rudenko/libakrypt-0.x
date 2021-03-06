/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_bckey.h                                                                                */
/*  - содержит описания функций, реализующих алгоритмы блочного шифрования.                        */
/* ----------------------------------------------------------------------------------------------- */
 #ifndef __AK_BCKEY_H__
 #define __AK_BCKEY_H__

/* ----------------------------------------------------------------------------------------------- */
 #include <ak_skey.h>
 #include <ak_gf2n.h>

/* ----------------------------------------------------------------------------------------------- */
/* предварительное описание */
 struct bckey;
/*! \brief Указатель на структуру ключа блочного алгоритма шифрования. */
 typedef struct bckey *ak_bckey;

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Функция создания ключа блочного алгоритма шифрования. */
 typedef int ( ak_function_bckey_create ) ( ak_bckey );
/*! \brief Функция зашифрования/расширования одного блока информации. */
 typedef void ( ak_function_bckey )( ak_skey, ak_pointer, ak_pointer );
/*! \brief Функция, предназначенная для зашифрования/расшифрования области памяти заданного размера */
 typedef int ( ak_function_bckey_encrypt )( ak_bckey, ak_pointer, ak_pointer, size_t,
                                                                                ak_pointer, size_t );
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Секретный ключ блочного алгоритма шифрования. */
 struct bckey {
  /*! \brief Указатель на секретный ключ. */
   struct skey key;
  /*! \brief Размер блока обрабатываемых данных (в байтах). */
   size_t bsize;
  /*! \brief Буффер, для хранения текущего значения синхропосылки.
      Длина буффера должна быть кратна длине блока. */
   struct buffer ivector;
  /*! \brief Функция заширования одного блока информации. */
   ak_function_bckey *encrypt;
  /*! \brief Функция расширования одного блока информации. */
   ak_function_bckey *decrypt;
  /*! \brief Функция развертки ключа. */
   ak_function_skey *schedule_keys;
  /*! \brief Функция уничтожения развернутых ключей. */
   ak_function_skey *delete_keys;
};

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Инициализация ключа произвольного алгоритма блочного шифрования. */
 int ak_bckey_context_create( ak_bckey , size_t , size_t );
/*! \brief Очистка ключа алгоритма блочного шифрования. */
 int ak_bckey_context_destroy( ak_bckey );
/*! \brief Удаление ключа алгоритма блочного шифрования. */
 ak_pointer ak_bckey_context_delete( ak_pointer );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Инициализация контекста секретного ключа алгоритма блочного шифрования Магма. */
 int ak_bckey_context_create_magma( ak_bckey );
/*! \brief Инициализация контекста секретного ключа алгоритма блочного шифрования Кузнечик. */
 int ak_bckey_context_create_kuznechik( ak_bckey );
/*! \brief Инициализация контекста секретного ключа алгоритма блочного шифрования по его OID. */
 int ak_bckey_context_create_oid( ak_bckey , ak_oid );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Присвоение контексту ключа алгоритма блочного шифрования константного значения. */
 int ak_bckey_context_set_key( ak_bckey, const ak_pointer , const size_t , const bool_t );
/*! \brief Присвоение контексту ключа алгоритма блочного шифрования случайного значения. */
 int ak_bckey_context_set_key_random( ak_bckey , ak_random );
/*! \brief Присвоение контексту ключа алгоритма блочного шифрования значения, выработанного из пароля. */
 int ak_bckey_context_set_key_from_password( ak_bckey , const ak_pointer , const size_t ,
                                                                const ak_pointer , const size_t );
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Инициализация ключа алгоритма блочного шифрования значением другого ключа */
 int ak_bckey_context_create_and_set_bckey( ak_bckey , ak_bckey );
/*! \brief Процедура вычисления производного ключа в соответствии с алгоритмом ACPKM
    из рекомендаций Р 1323565.1.012-2018. */
 int ak_bckey_context_next_acpkm_key( ak_bckey );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Зашифрование данных в режиме простой замены. */
 int ak_bckey_context_encrypt_ecb( ak_bckey , ak_pointer , ak_pointer , size_t );
/*! \brief Расшифрование данных в режиме простой замены. */
 int ak_bckey_context_decrypt_ecb( ak_bckey , ak_pointer , ak_pointer , size_t );
/*! \brief Шифрование данных в режиме гаммирования из ГОСТ Р 34.13-2015. */
 int ak_bckey_context_ctr( ak_bckey , ak_pointer , ak_pointer , size_t , ak_pointer , size_t );
/*! \brief Шифрование данных в режиме CTR-ACPKM из Р 1323565.1.017—2018. */
 int ak_bckey_context_ctr_acpkm( ak_bckey , ak_pointer , ak_pointer , size_t , size_t ,
                                                                           ak_pointer , size_t );
/*! \brief Вычисление имитовставки согласно ГОСТ Р 34.13-2015. */
 ak_buffer ak_bckey_context_omac( ak_bckey , ak_pointer , size_t , ak_pointer );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Зашифрование данных в режиме MGM с одновременной выработкой имитовставки. */
 ak_buffer ak_bckey_context_encrypt_mgm( ak_bckey , ak_bckey , const ak_pointer , const size_t ,
                   const ak_pointer , ak_pointer , const size_t , const ak_pointer , const size_t ,
                                                                         ak_pointer , const size_t );
/*! \brief Расшифрование данных в режиме MGM с одновременной проверкой имитовставки. */
 bool_t ak_bckey_context_decrypt_mgm( ak_bckey , ak_bckey , const ak_pointer , const size_t ,
                   const ak_pointer , ak_pointer , const size_t , const ak_pointer , const size_t ,
                                                                          ak_pointer, const size_t );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Инициализация внутренних структур данных, используемых при реализации алгоритма
    блочного шифрования Кузнечик (ГОСТ Р 34.12-2015). */
 bool_t ak_bckey_init_kuznechik_tables( void );
/*! \brief Тестирование корректной работы алгоритма блочного шифрования Магма (ГОСТ Р 34.12-2015). */
 bool_t ak_bckey_test_magma( void );
/*! \brief Тестирование корректной работы алгоритма блочного шифрования Кузнечик (ГОСТ Р 34.12-2015). */
 bool_t ak_bckey_test_kuznechik( void );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Тестирование корректной работы режима блочного шифрования с одновременной
    выработкой имитовставки. */
 bool_t ak_bckey_test_mgm( void );
/*! \brief Тестирование корректной работы режима шифрования ACPKM, регламентируемого Р 1323565.1.017—2018. */
 bool_t ak_bckey_test_acpkm( void );

 #endif
/* ----------------------------------------------------------------------------------------------- */
/*                                                                                     ak_bckey.h  */
/* ----------------------------------------------------------------------------------------------- */
