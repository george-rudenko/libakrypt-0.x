/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2017 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_mac.h                                                                                  */
/*  - содержит описания функций, реализующих аутентифицированное шифрование
      и различные режимы его применения.                                                           */
/* ----------------------------------------------------------------------------------------------- */
#ifndef __AK_MGM_H__
#define __AK_MGM_H__

/* ----------------------------------------------------------------------------------------------- */
 #include <ak_bckey.h>

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Структура, содержащая текущее состояние внутренних переменных режима
   аутентифицированного шифрования. */
 typedef __attribute__((aligned(16))) struct mgm_ctx {
  /*! \brief Текущее значение имитовставки. */
   ak_uint128 sum;
  /*! \brief Счетчик, значения которого используются при шифровании информации. */
   ak_uint128 ycount;
  /*! \brief Счетчик, значения которого используются при выработке имитовставки. */
   ak_uint128 zcount;
  /*! \brief Размер обработанных зашифровываемых/расшифровываемых данных в битах. */
   ssize_t pbitlen;
  /*! \brief Размер обработанных дополнительных данных в битах. */
   ssize_t abitlen;
  /*! \brief Флаги состояния контекста. */
   ak_uint32 flags;
} *ak_mgm_ctx;

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Секретный ключ алгоритма выработки имитовставки, входящего в режим
   аутентифицированного шифрования. */
 typedef __attribute__((aligned(16))) struct mgm {
 /*! \brief Контекст секретного ключа аутентификации. */
  struct bckey bkey;
 /*! \brief Текущее состояние внутренних переменных алгоритма аутентифицированного шифрования. */
  struct mgm_ctx mctx;
 /*! \brief Вектор со значением инициализационного вектора. */
  struct buffer iv;
} *ak_mgm;

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Инициализация начального значения внутреннего состояния алгоритма MGM перед
    обработкой дополнительных данных. */
 int ak_mgm_context_authentication_clean( ak_mgm_ctx , ak_bckey , const ak_pointer , const size_t );
/*! \brief Изменение значения внутреннего состояния алгоритма MGM при обработке дополнительных данных. */
 int ak_mgm_context_authentication_update( ak_mgm_ctx, ak_bckey , const ak_pointer , const size_t );
/*! \brief Завершение действий и вычисление имитовставки. */
 ak_buffer ak_mgm_context_authentication_finalize( ak_mgm_ctx ,
                                                              ak_bckey , ak_pointer, const size_t );
/*! \brief Инициализация начального значения счетчика для шифрования. */
 int ak_mgm_context_encryption_clean( ak_mgm_ctx , ak_bckey , const ak_pointer , const size_t );
/*! \brief Зашифрование данных и обновление внутреннего состояния счетчика для шифрования. */
 int ak_mgm_context_encryption_update( ak_mgm_ctx , ak_bckey ,
                                          ak_bckey , const ak_pointer , ak_pointer , const size_t );
/*! \brief Расшифрование данных и обновление внутреннего состояния счетчика для шифрования. */
 int ak_mgm_context_decryption_update( ak_mgm_ctx , ak_bckey ,
                                          ak_bckey , const ak_pointer , ak_pointer , const size_t );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Зашифрование данных в режиме MGM с одновременной выработкой имитовставки. */
 ak_buffer ak_bckey_context_encrypt_mgm( ak_bckey , ak_bckey , const ak_pointer , const size_t ,
                   const ak_pointer , ak_pointer , const size_t , const ak_pointer , const size_t ,
                                                                         ak_pointer , const size_t );
/*! \brief Расшифрование данных в режиме MGM с одновременной проверкой имитовставки. */
 ak_bool ak_bckey_context_decrypt_mgm( ak_bckey , ak_bckey , const ak_pointer , const size_t ,
                   const ak_pointer , ak_pointer , const size_t , const ak_pointer , const size_t ,
                                                                          ak_pointer, const size_t );
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Инициализация конектста алгоритма выработки имитовставки MGM
     на основе блочного шифра Магма. */
 int ak_mgm_context_create_magma( ak_mgm );
/*! \brief Инициализация конектста алгоритма выработки имитовставки MGM
     на основе блочного шифра Кузнечик. */
 int ak_mgm_context_create_kuznechik( ak_mgm );
/*! \brief Создание контекста алгоритма вырабтки имитовставки MGM c помощью заданного oid. */
 int ak_mgm_context_create_oid( ak_mgm , ak_oid );
/*! \brief Уничтожение контекста алгоритма выработки имитовставки MGM. */
 int ak_mgm_context_destroy( ak_mgm );
/*! \brief Освобождение памяти из под контекста алгоритма выработки имитовставки MGM. */
 ak_pointer ak_mgm_context_delete( ak_pointer );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Присвоение контексту заданного значения инициализационного вектора (синхропосылки). */
 int ak_mgm_context_set_iv( ak_mgm , const ak_pointer , const size_t );
/*! \brief Присвоение секретному ключу константного значения. */
 int ak_mgm_context_set_key( ak_mgm , const ak_pointer , const size_t , const ak_bool );
/*! \brief Присвоение секретному ключу случайного значения. */
 int ak_mgm_context_set_key_random( ak_mgm , ak_random );
/*! \brief Присвоение секретному ключу значения, выработанного из пароля */
 int ak_mgm_context_set_key_from_password( ak_mgm , const ak_pointer , const size_t ,
                                                                 const ak_pointer , const size_t );
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Очистка контекста алгоритма выработки имитовставки MGM. */
 int ak_mgm_context_clean( ak_pointer );
/*! \brief Обновление текущего состояния контекста алгоритма выработки имитовставки MGM. */
 int ak_mgm_context_update( ak_pointer , const ak_pointer , const size_t );
/*! \brief Завершение алгоритма выработки имитовставки MGM. */
 ak_buffer ak_mgm_context_finalize( ak_pointer , const ak_pointer , const size_t , ak_pointer );

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Тестирование корректной работы режима блочного шифрования с одновременной
    выработкой имитовставки. */
 ak_bool ak_bckey_test_mgm( void );

#endif
/* ----------------------------------------------------------------------------------------------- */
/*                                                                                       ak_mgm.h  */
/* ----------------------------------------------------------------------------------------------- */
