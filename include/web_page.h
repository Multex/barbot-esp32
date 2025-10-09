#pragma once
#include <Arduino.h>

const char PAGE_MAIN[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BarBot Control</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Arial', sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; color: white; overflow-x: hidden; }
        .container { max-width: 500px; margin: 0 auto; padding: 20px; }
        .header { text-align: center; margin-bottom: 30px; padding: 20px; background: rgba(255, 255, 255, 0.1); border-radius: 15px; backdrop-filter: blur(10px); box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1); }
        .header h1 { font-size: 2.5em; margin-bottom: 10px; text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3); }
        .status-panel { background: rgba(255, 255, 255, 0.15); border-radius: 15px; padding: 20px; margin-bottom: 20px; text-align: center; backdrop-filter: blur(10px); box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1); }
        .status-indicator { width: 20px; height: 20px; border-radius: 50%; display: inline-block; margin-right: 10px; animation: pulse 2s infinite; }
        .status-idle { background-color: #4CAF50; } .status-preparing, .status-recipe_start, .status-recipe_move, .status-recipe_serve { background-color: #FF9800; } .status-recipe_wait_finish { background-color: #4CAF50; } .status-recipe_return { background-color: #2196F3; } .status-homing { background-color: #00BCD4; }
        @keyframes pulse { 0% { box-shadow: 0 0 0 0 rgba(255, 255, 255, 0.7); } 70% { box-shadow: 0 0 0 10px rgba(255, 255, 255, 0); } 100% { box-shadow: 0 0 0 0 rgba(255, 255, 255, 0); } }
        .category-section { margin-bottom: 25px; background: rgba(255, 255, 255, 0.1); border-radius: 15px; overflow: hidden; backdrop-filter: blur(10px); box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1); }
        .category-header { background: rgba(255, 255, 255, 0.2); padding: 20px; cursor: pointer; display: flex; justify-content: space-between; align-items: center; font-size: 1.2em; font-weight: bold; }
        .category-icon { font-size: 1.5em; transition: transform 0.3s ease; }
        .category-icon.collapsed { transform: rotate(-90deg); }
        .recipes-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; padding: 20px; max-height: 0; overflow: hidden; transition: max-height 0.5s ease-out; }
        .recipes-grid.expanded { max-height: 2000px; }
        .recipe-btn { background: rgba(255, 255, 255, 0.2); border: none; border-radius: 15px; padding: 15px; color: white; cursor: pointer; transition: all 0.3s ease; backdrop-filter: blur(10px); min-height: 90px; display: flex; flex-direction: column; align-items: center; justify-content: center; text-align: center; }
        .recipe-btn:hover:enabled { background: rgba(255, 255, 255, 0.3); transform: translateY(-2px); box-shadow: 0 12px 40px rgba(0, 0, 0, 0.2); }
        .recipe-btn:disabled { opacity: 0.5; cursor: not-allowed; }
        .recipe-name { font-size: 1.1em; font-weight: bold; }
        .recipe-ingredients { font-size: 0.8em; margin-top: 5px; font-weight: normal; opacity: 0.8; }
        .loading { display: none; text-align: center; margin: 20px 0; }
        .spinner { border: 4px solid rgba(255, 255, 255, 0.3); border-top: 4px solid white; border-radius: 50%; width: 40px; height: 40px; animation: spin 1s linear infinite; margin: 0 auto 10px; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        .modal-overlay { position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(0, 0, 0, 0.7); display: flex; align-items: center; justify-content: center; z-index: 1000; backdrop-filter: blur(5px); }
        .modal-content { background: linear-gradient(135deg, #764ba2, #667eea); padding: 30px; border-radius: 15px; text-align: center; box-shadow: 0 10px 40px rgba(0,0,0,0.2); width: 90%; max-width: 400px; }
        .modal-content h2 { margin-bottom: 15px; }
        .modal-content p { margin-bottom: 25px; font-size: 1.1em; }
        .modal-buttons { display: flex; justify-content: space-around; }
        .modal-btn { border: none; padding: 12px 25px; border-radius: 10px; font-size: 1em; font-weight: bold; cursor: pointer; transition: all 0.2s ease; color: white; }
        .modal-btn.confirm { background-color: #4CAF50; }
        .modal-btn.confirm:hover { background-color: #45a049; }
        .modal-btn.cancel { background-color: #f44336; }
        .modal-btn.cancel:hover { background-color: #e53935; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header"><h1>🍹 BarBot</h1></div>
        <div class="status-panel"><div class="status-indicator" id="statusIndicator"></div><span id="statusText"></span></div>
        <div class="loading" id="loading"><div class="spinner"></div><p id="loadingText">Preparando...</p></div>
        <div id="controls">
            <div class="category-section">
                <div class="category-header" onclick="toggleCategory('alcohol')"><span>🍸 Con Alcohol</span><span class="category-icon" id="alcoholIcon"></span></div>
                <div class="recipes-grid" id="alcoholGrid"></div>
            </div>
            <div class="category-section">
                <div class="category-header" onclick="toggleCategory('nonalcohol')"><span>🥤 Sin Alcohol</span><span class="category-icon" id="nonalcoholIcon"></span></div>
                <div class="recipes-grid" id="nonalcoholGrid"></div>
            </div>
        </div>
    </div>
    <div id="confirmationModal" class="modal-overlay" style="display:none;">
        <div class="modal-content">
            <h2>Confirmar Preparación</h2>
            <p>¿Deseas preparar <strong id="confirmRecipeName"></strong>?</p>
            <div class="modal-buttons">
                <button id="confirmBtn" class="modal-btn confirm">Confirmar</button>
                <button id="cancelBtn" class="modal-btn cancel">Cancelar</button>
            </div>
        </div>
    </div>
    <script>
        const confirmationModal = document.getElementById('confirmationModal');
        const confirmRecipeName = document.getElementById('confirmRecipeName');
        const confirmBtn = document.getElementById('confirmBtn');
        const cancelBtn = document.getElementById('cancelBtn');
        let recipeToPrepare = '';
        const statusIndicator = document.getElementById('statusIndicator');
        const statusText = document.getElementById('statusText');
        const loadingDiv = document.getElementById('loading');
        const loadingText = document.getElementById('loadingText');
        const controlsDiv = document.getElementById('controls');
        function updateStatus() {
            fetch('/status').then(response => response.json()).then(data => {
                statusText.textContent = data.message;
                let stateClass = data.state.toLowerCase();
                if(stateClass.includes('recipe')) stateClass = 'preparing';
                statusIndicator.className = 'status-indicator status-' + stateClass;
                const isBusy = data.state !== 'IDLE';
                loadingDiv.style.display = isBusy ? 'block' : 'none';
                loadingText.textContent = data.message;
                controlsDiv.style.opacity = isBusy ? '0.5' : '1';
                document.querySelectorAll('.recipe-btn').forEach(btn => btn.disabled = isBusy);
            }).catch(error => console.error('Error actualizando estado:', error));
        }
        function showConfirmation(recipeName) {
            recipeToPrepare = recipeName;
            confirmRecipeName.textContent = friendlyNames[recipeName] || recipeName;
            confirmationModal.style.display = 'flex';
        }
        function executePreparation() {
            if (!recipeToPrepare) return;
            fetch('/prepare', { method: 'POST', headers: { 'Content-Type': 'application/x-www-form-urlencoded', }, body: 'recipe=' + encodeURIComponent(recipeToPrepare) })
            .then(response => response.text()).then(data => { console.log('Respuesta de /prepare:', data); updateStatus(); })
            .catch(error => console.error('Error al preparar:', error));
            confirmationModal.style.display = 'none';
            recipeToPrepare = '';
        }
        function toggleCategory(category) {
            const grid = document.getElementById(category + 'Grid');
            const icon = document.getElementById(category + 'Icon');
            grid.classList.toggle('expanded');
            icon.classList.toggle('collapsed');
            icon.textContent = grid.classList.contains('expanded') ? '🔽' : '▶️';
        }
        const friendlyNames = { 'CubaLibre': 'Cuba Libre', 'RonConNaranja': 'Ron con Naranja', 'RonTonic': 'Ron Tonic', 'Screwdriver': 'Screwdriver', 'VodkaTonic': 'Vodka Tonic', 'VodkaCola': 'Vodka con Coca-Cola', 'RonVodkaCola': 'Ron & Vodka Cola', 'RonVodkaNaranja': 'Vodka & Ron Cítrus', 'RonVodkaTonic': 'Ron & Vodka Tonic', 'ShotVodka': 'Shot de Vodka', 'ShotRon': 'Shot de Ron', 'Shot5050': 'Shot 50/50', 'ShotCitrico': 'Shot Cítrico', 'NaranjaTonic': 'Naranja Tónica', 'CitricoCola': 'Cítrico Cola', 'VodkaSunriseFake': 'Vodka Sunrise Fake', 'RonFresh': 'Ron Fresh' };
        const ingredientsMap = { 'CubaLibre': 'Ron, Coca-Cola', 'RonConNaranja': 'Ron, Jugo de naranja', 'RonTonic': 'Ron, Agua tónica', 'Screwdriver': 'Vodka, Jugo de naranja', 'VodkaTonic': 'Vodka, Agua tónica', 'VodkaCola': 'Vodka, Coca-Cola', 'RonVodkaCola': 'Ron, Vodka, Coca-Cola', 'RonVodkaNaranja': 'Ron, Vodka, Jugo de naranja', 'RonVodkaTonic': 'Ron, Vodka, Agua tónica', 'ShotVodka': 'Vodka', 'ShotRon': 'Ron', 'Shot5050': 'Vodka, Ron', 'ShotCitrico': 'Vodka, Jugo de naranja', 'NaranjaTonic': 'Jugo de naranja, Agua tónica', 'CitricoCola': 'Jugo de naranja, Coca-Cola', 'VodkaSunriseFake': 'Vodka, Jugo de naranja, Coca-Cola', 'RonFresh': 'Ron, Jugo de naranja, Agua tónica' };
        const alcoholicRecipes = [ 'CubaLibre', 'RonConNaranja', 'RonTonic', 'Screwdriver', 'VodkaTonic', 'VodkaCola', 'RonVodkaCola', 'RonVodkaNaranja', 'RonVodkaTonic', 'ShotVodka', 'ShotRon', 'Shot5050', 'ShotCitrico', 'VodkaSunriseFake', 'RonFresh' ];
        function loadRecipes() {
            fetch('/recipes').then(response => response.json()).then(recipes => {
                const alcoholGrid = document.getElementById('alcoholGrid');
                const nonAlcoholGrid = document.getElementById('nonalcoholGrid');
                alcoholGrid.innerHTML = '';
                nonAlcoholGrid.innerHTML = '';
                for (const recipeName of Object.keys(recipes)) {
                    const button = document.createElement('button');
                    button.className = 'recipe-btn';
                    button.onclick = () => showConfirmation(recipeName);
                    const nameSpan = document.createElement('span');
                    nameSpan.className = 'recipe-name';
                    nameSpan.textContent = friendlyNames[recipeName] || recipeName;
                    const ingredientsSpan = document.createElement('span');
                    ingredientsSpan.className = 'recipe-ingredients';
                    ingredientsSpan.textContent = ingredientsMap[recipeName] || '';
                    button.appendChild(nameSpan);
                    button.appendChild(ingredientsSpan);
                    if (alcoholicRecipes.includes(recipeName)) {
                        alcoholGrid.appendChild(button);
                    } else {
                        nonAlcoholGrid.appendChild(button);
                    }
                }
            }).catch(error => console.error('Error cargando recetas:', error));
        }
        document.addEventListener('DOMContentLoaded', () => {
            loadRecipes();
            updateStatus();
            toggleCategory('alcohol');
            toggleCategory('nonalcohol');
            setInterval(updateStatus, 2000);
            confirmBtn.addEventListener('click', executePreparation);
            cancelBtn.addEventListener('click', () => {
                confirmationModal.style.display = 'none';
                recipeToPrepare = '';
            });
        });
    </script>
</body>
</html>
)rawliteral";