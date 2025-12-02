#pragma once
#include <Arduino.h>

const char PAGE_MAIN[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BarBot Premium</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;500;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --primary: #6366f1;
            --secondary: #a855f7;
            --accent: #ec4899;
            --glass-bg: rgba(255, 255, 255, 0.1);
            --glass-border: rgba(255, 255, 255, 0.2);
            --text-color: #ffffff;
        }

        * { margin: 0; padding: 0; box-sizing: border-box; }
        
        body {
            font-family: 'Outfit', sans-serif;
            background: linear-gradient(135deg, #0f172a 0%, #1e1b4b 50%, #312e81 100%);
            min-height: 100vh;
            color: var(--text-color);
            overflow-x: hidden;
        }

        .container {
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
        }

        /* Glassmorphism Header */
        .header {
            text-align: center;
            margin-bottom: 30px;
            padding: 30px;
            background: var(--glass-bg);
            border: 1px solid var(--glass-border);
            border-radius: 24px;
            backdrop-filter: blur(12px);
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
        }

        .header h1 {
            font-size: 3em;
            font-weight: 700;
            background: linear-gradient(to right, #818cf8, #c084fc, #f472b6);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            margin-bottom: 5px;
        }

        .header p {
            font-size: 1.1em;
            opacity: 0.8;
            font-weight: 300;
        }

        /* Status Panel */
        .status-panel {
            background: rgba(15, 23, 42, 0.6);
            border: 1px solid var(--glass-border);
            border-radius: 20px;
            padding: 20px;
            margin-bottom: 30px;
            display: flex;
            align-items: center;
            justify-content: center;
            backdrop-filter: blur(10px);
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.2);
            transition: all 0.3s ease;
        }

        .status-indicator {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            margin-right: 15px;
            box-shadow: 0 0 15px currentColor;
        }

        .status-idle { background-color: #10b981; color: #10b981; }
        .status-busy { background-color: #f59e0b; color: #f59e0b; animation: pulse 1.5s infinite; }

        @keyframes pulse {
            0% { transform: scale(1); opacity: 1; }
            50% { transform: scale(1.2); opacity: 0.7; }
            100% { transform: scale(1); opacity: 1; }
        }

        #statusText {
            font-weight: 500;
            letter-spacing: 0.5px;
        }

        /* Category Tabs */
        .tabs {
            display: flex;
            justify-content: center;
            gap: 15px;
            margin-bottom: 25px;
        }

        .tab-btn {
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid var(--glass-border);
            padding: 12px 25px;
            border-radius: 50px;
            color: white;
            font-family: inherit;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.3s ease;
        }

        .tab-btn.active {
            background: linear-gradient(135deg, var(--primary), var(--secondary));
            border-color: transparent;
            box-shadow: 0 4px 15px rgba(99, 102, 241, 0.4);
        }

        /* Recipe Grid */
        .recipes-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(160px, 1fr));
            gap: 20px;
            padding-bottom: 40px;
        }

        .recipe-card {
            background: var(--glass-bg);
            border: 1px solid var(--glass-border);
            border-radius: 20px;
            padding: 20px;
            cursor: pointer;
            transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
            position: relative;
            overflow: hidden;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            min-height: 160px;
        }

        .recipe-card::before {
            content: '';
            position: absolute;
            top: 0; left: 0; right: 0; bottom: 0;
            background: linear-gradient(135deg, rgba(255,255,255,0.1), transparent);
            opacity: 0;
            transition: opacity 0.3s ease;
        }

        .recipe-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
            border-color: rgba(255, 255, 255, 0.4);
        }

        .recipe-card:hover::before {
            opacity: 1;
        }

        .recipe-icon {
            font-size: 2.5em;
            margin-bottom: 15px;
        }

        .recipe-name {
            font-size: 1.1em;
            font-weight: 700;
            margin-bottom: 5px;
            line-height: 1.2;
        }

        .recipe-ingredients {
            font-size: 0.8em;
            opacity: 0.6;
            font-weight: 300;
        }

        /* Modal */
        .modal-overlay {
            position: fixed;
            top: 0; left: 0; width: 100%; height: 100%;
            background: rgba(0, 0, 0, 0.8);
            backdrop-filter: blur(8px);
            display: flex;
            align-items: center;
            justify-content: center;
            z-index: 1000;
            opacity: 0;
            pointer-events: none;
            transition: opacity 0.3s ease;
        }

        .modal-overlay.active {
            opacity: 1;
            pointer-events: all;
        }

        .modal-content {
            background: #1e293b;
            border: 1px solid var(--glass-border);
            padding: 40px;
            border-radius: 30px;
            text-align: center;
            width: 90%;
            max-width: 400px;
            transform: scale(0.9);
            transition: transform 0.3s cubic-bezier(0.34, 1.56, 0.64, 1);
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
        }

        .modal-overlay.active .modal-content {
            transform: scale(1);
        }

        .modal-title {
            font-size: 1.8em;
            font-weight: 700;
            margin-bottom: 10px;
            color: white;
        }

        .modal-desc {
            color: #94a3b8;
            margin-bottom: 30px;
        }

        .modal-buttons {
            display: flex;
            gap: 15px;
            justify-content: center;
        }

        .btn {
            padding: 12px 30px;
            border-radius: 15px;
            font-weight: 600;
            cursor: pointer;
            border: none;
            transition: transform 0.2s ease;
        }

        .btn:active { transform: scale(0.95); }

        .btn-confirm {
            background: linear-gradient(135deg, #10b981, #059669);
            color: white;
            box-shadow: 0 4px 15px rgba(16, 185, 129, 0.4);
        }

        .btn-cancel {
            background: rgba(255, 255, 255, 0.1);
            color: white;
        }

        /* Loading Spinner */
        .loader {
            width: 48px;
            height: 48px;
            border: 5px solid #FFF;
            border-bottom-color: transparent;
            border-radius: 50%;
            display: inline-block;
            box-sizing: border-box;
            animation: rotation 1s linear infinite;
            margin-bottom: 20px;
        }

        @keyframes rotation {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }

        .preparing-overlay {
            position: fixed;
            top: 0; left: 0; width: 100%; height: 100%;
            background: rgba(15, 23, 42, 0.95);
            z-index: 2000;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            transition: opacity 0.5s ease;
        }
        
        .hidden { display: none !important; opacity: 0; }

    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>BarBot</h1>
            <p>Tu bartender personal</p>
        </div>

        <div class="status-panel">
            <div id="statusIndicator" class="status-indicator status-idle"></div>
            <span id="statusText">Conectando...</span>
        </div>

        <div class="tabs">
            <button class="tab-btn active" onclick="filterRecipes('alcohol')">Con Alcohol</button>
            <button class="tab-btn" onclick="filterRecipes('non-alcohol')">Sin Alcohol</button>
        </div>

        <div id="recipesGrid" class="recipes-grid">
            <!-- Recipes injected here -->
        </div>
    </div>

    <!-- Confirmation Modal -->
    <div id="confirmModal" class="modal-overlay">
        <div class="modal-content">
            <div style="font-size: 3em; margin-bottom: 20px;">🍹</div>
            <h2 class="modal-title" id="modalRecipeName">Confirmar</h2>
            <p class="modal-desc">¿Deseas preparar esta bebida?</p>
            <div class="modal-buttons">
                <button class="btn btn-cancel" onclick="closeModal()">Cancelar</button>
                <button class="btn btn-confirm" onclick="confirmPreparation()">¡Servir!</button>
            </div>
        </div>
    </div>

    <!-- Preparing Overlay -->
    <div id="preparingOverlay" class="preparing-overlay hidden">
        <span class="loader"></span>
        <h2 style="font-size: 1.5em; margin-bottom: 10px;">Preparando...</h2>
        <p id="preparingStatus" style="color: #94a3b8;">Por favor espera</p>
    </div>

    <script>
        let allRecipes = [];
        let selectedRecipeId = null;
        let currentCategory = 'alcohol';

        // Elements
        const grid = document.getElementById('recipesGrid');
        const modal = document.getElementById('confirmModal');
        const overlay = document.getElementById('preparingOverlay');
        const statusText = document.getElementById('statusText');
        const statusIndicator = document.getElementById('statusIndicator');
        const preparingStatus = document.getElementById('preparingStatus');

        // Init
        document.addEventListener('DOMContentLoaded', () => {
            fetchRecipes();
            setInterval(updateStatus, 2000);
            updateStatus();
        });

        function fetchRecipes() {
            fetch('/recipes')
                .then(res => res.json())
                .then(data => {
                    allRecipes = data.recipes || [];
                    renderRecipes();
                })
                .catch(err => {
                    console.error('Error loading recipes:', err);
                    statusText.textContent = "Error cargando recetas";
                });
        }

        function renderRecipes() {
            grid.innerHTML = '';
            const filtered = allRecipes.filter(r => r.category === currentCategory);
            
            filtered.forEach(r => {
                const card = document.createElement('div');
                card.className = 'recipe-card';
                card.onclick = () => openModal(r);
                
                const icon = r.category === 'alcohol' ? '🍸' : '🥤';
                
                card.innerHTML = `
                    <div class="recipe-icon">${icon}</div>
                    <div>
                        <div class="recipe-name">${r.name}</div>
                        <div class="recipe-ingredients">${r.ingredients}</div>
                    </div>
                `;
                grid.appendChild(card);
            });
        }

        function filterRecipes(category) {
            currentCategory = category;
            
            // Update tabs
            document.querySelectorAll('.tab-btn').forEach(btn => {
                if(btn.textContent.toLowerCase().includes(category === 'alcohol' ? 'con' : 'sin')) {
                    btn.classList.add('active');
                } else {
                    btn.classList.remove('active');
                }
            });
            
            renderRecipes();
        }

        function openModal(recipe) {
            selectedRecipeId = recipe.id;
            document.getElementById('modalRecipeName').textContent = recipe.name;
            modal.classList.add('active');
        }

        function closeModal() {
            modal.classList.remove('active');
            selectedRecipeId = null;
        }

        function confirmPreparation() {
            if(!selectedRecipeId) return;
            
            const recipeId = selectedRecipeId; // Capture ID before clearing
            
            // Optimistic Update: Show blocking overlay immediately
            closeModal();
            statusIndicator.className = 'status-indicator status-busy';
            overlay.classList.remove('hidden');
            preparingStatus.textContent = "Iniciando preparación...";
            
            fetch('/prepare', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: 'recipe=' + encodeURIComponent(recipeId)
            })
            .then(response => {
                if (!response.ok) throw new Error('Server returned ' + response.status);
                return response.text();
            })
            .catch(err => {
                console.error('Error starting preparation:', err);
                // Revert optimistic update on error
                statusIndicator.className = 'status-indicator status-idle';
                overlay.classList.add('hidden');
                alert("Error: " + err.message);
            });
        }

        function updateStatus() {
            fetch('/status')
                .then(res => res.json())
                .then(data => {
                    // Update main status
                    statusText.textContent = data.message;
                    
                    const isBusy = data.state !== 'IDLE';
                    
                    if(isBusy) {
                        statusIndicator.className = 'status-indicator status-busy';
                        overlay.classList.remove('hidden');
                        preparingStatus.textContent = data.message;
                    } else {
                        statusIndicator.className = 'status-indicator status-idle';
                        overlay.classList.add('hidden');
                    }
                })
                .catch(console.error);
        }
    </script>
</body>
</html>
)rawliteral";