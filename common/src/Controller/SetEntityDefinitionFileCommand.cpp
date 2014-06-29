/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SetEntityDefinitionFileCommand.h"

#include "Model/Entity.h"
#include "Model/EntityProperties.h"
#include "View/MapDocument.h"

namespace TrenchBroom {
    namespace Controller {
        const Command::CommandType SetEntityDefinitionFileCommand::Type = Command::freeType();

        SetEntityDefinitionFileCommand::Ptr SetEntityDefinitionFileCommand::setEntityDefinitionFileSpec(View::MapDocumentWPtr document, const Model::EntityDefinitionFileSpec& spec) {
            return Ptr(new SetEntityDefinitionFileCommand(document, spec));
        }

        SetEntityDefinitionFileCommand::SetEntityDefinitionFileCommand(View::MapDocumentWPtr document, const Model::EntityDefinitionFileSpec& spec) :
        Command(Type, "Set Entity Definition File", true, true),
        m_document(document),
        m_newSpec(spec) {}
        
        bool SetEntityDefinitionFileCommand::doPerformDo() {
            View::MapDocumentSPtr document = lock(m_document);
            Model::Entity* worldspawn = document->worldspawn();
            document->objectWillChangeNotifier(worldspawn);
            m_oldSpec = document->entityDefinitionFile();
            worldspawn->addOrUpdateProperty(Model::PropertyKeys::EntityDefinitions, m_newSpec.asString());
            document->objectDidChangeNotifier(worldspawn);
            document->entityDefinitionsDidChangeNotifier();
            return true;
        }
        
        bool SetEntityDefinitionFileCommand::doPerformUndo() {
            View::MapDocumentSPtr document = lock(m_document);
            Model::Entity* worldspawn = document->worldspawn();
            document->objectWillChangeNotifier(worldspawn);
            worldspawn->addOrUpdateProperty(Model::PropertyKeys::EntityDefinitions, m_oldSpec.asString());
            document->objectDidChangeNotifier(worldspawn);
            document->entityDefinitionsDidChangeNotifier();
            return true;
        }

        bool SetEntityDefinitionFileCommand::doCollateWith(Command::Ptr command) {
            return false;
        }
    }
}
