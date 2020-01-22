/*
 * TranslationsLoader.cpp - implementation of TranslationsLoader class
 *
 * Copyright (c) 2020 Tobias Junghans <tobydox@veyon.io>
 *
 * This file is part of Veyon - https://veyon.io
 *
 */

#include <QCoreApplication>
#include <QTranslator>

#include "TranslationsLoader.h"
#include "VeyonConfiguration.h"
#include "VeyonCore.h"


TranslationsLoader::TranslationsLoader( const QString& resourceName )
{
	if( VeyonCore::instance()->findChild<QTranslator *>( resourceName ) )
	{
		return;
	}

	QLocale configuredLocale( QLocale::C );

	QRegExp localeRegEx( QStringLiteral( "[^(]*\\(([^)]*)\\)") );
	if( localeRegEx.indexIn( VeyonCore::config().uiLanguage() ) == 0 )
	{
		configuredLocale = QLocale( localeRegEx.cap( 1 ) );
	}

	if( configuredLocale.language() != QLocale::English )
	{
		auto translator = new QTranslator( VeyonCore::instance() );
		translator->setObjectName( resourceName );

		if( configuredLocale == QLocale::C ||
			translator->load( QStringLiteral( "%1_%2.qm" ).arg( resourceName, configuredLocale.name() ),
							  VeyonCore::translationsDirectory() ) == false )
		{
			configuredLocale = QLocale::system(); // Flawfinder: ignore

			if( translator->load( QStringLiteral( "%1_%2.qm" ).arg( resourceName, configuredLocale.name() ),
								  VeyonCore::translationsDirectory() ) == false )
			{
				translator->load( QStringLiteral( "%1_%2.qm" ).arg( resourceName, configuredLocale.language() ),
								  VeyonCore::translationsDirectory() );
			}
		}

		QLocale::setDefault( configuredLocale );

		QCoreApplication::installTranslator( translator );
	}
}
